package chat.server.connection;

import chat.serialization.StreamHandler;
import chat.serialization.packets.MessagePacket;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

public class MessageHandler {
    private StreamHandler streamHandler;
    private AtomicInteger messageNumber;
    private ConcurrentHashMap<String, StreamHandler> clientConnections;
    private ConcurrentSkipListSet<String> clientNames;
    private String clientName;

    public MessageHandler(StreamHandler streamHandler, AtomicInteger messageNumber,
                          ConcurrentHashMap<String, StreamHandler> clientConnections, ConcurrentSkipListSet<String> clientNames, String clientName) {
        this.streamHandler = streamHandler;
        this.messageNumber = messageNumber;
        this.clientConnections = clientConnections;
        this.clientNames = clientNames;
        this.clientName = clientName;
    }

    public void handle(List<AtomicReference<MessagePacket>> chatHistory) {
        while (streamHandler.getSocket().isConnected()) {
            try {
                MessagePacket receivedMessagePacket = streamHandler.getJsonSerializer().fromJson(streamHandler.getScanner().nextLine(), MessagePacket.class);
                if (receivedMessagePacket.getContent() != null) {
                    broadcast(receivedMessagePacket);
                    record(receivedMessagePacket, chatHistory);
                } else {
                    sendClientList();
                }
            } catch (Exception e) {
                break;
            }
        }
    }

    private void broadcast(MessagePacket messagePacket) {
        for (String name : clientNames) {
            if (!clientName.equals(name)) {
                StreamHandler receiver = clientConnections.get(name);
                receiver.getWriter().println(receiver.getJsonSerializer().toJson(messagePacket));
            }
        }
        streamHandler.getWriter().println(streamHandler.getJsonSerializer().toJson(messagePacket));
    }

    private void record(MessagePacket messagePacket, List<AtomicReference<MessagePacket>> chatHistory) {
        chatHistory.get(messageNumber.getAndAdd(1) % chatHistory.size()).set(messagePacket);
        messageNumber.set(messageNumber.get() % chatHistory.size());
    }

    private void sendClientList() {
        MessagePacket messagePacket = new MessagePacket();
        messagePacket.setClients(clientNames);
        streamHandler.getWriter().println(streamHandler.getJsonSerializer().toJson(messagePacket));
    }
}

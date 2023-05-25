package chat.server.connection;

import chat.serialization.StreamHandler;
import chat.serialization.packets.InitialPacket;
import chat.serialization.packets.MessagePacket;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

public class ClientInitializer {
    private StreamHandler clientStreamHandler;
    private final ConcurrentSkipListSet<String> clientNames;
    private final ConcurrentHashMap<String, StreamHandler> clientConnectionsMap;
    String name;
    public ClientInitializer(StreamHandler streamHandler, ConcurrentSkipListSet<String> names, ConcurrentHashMap<String, StreamHandler> connectionsMap) {
        clientStreamHandler = streamHandler;
        clientNames = names;
        clientConnectionsMap = connectionsMap;
    }

    public String initialize() {
        while (true) {
            InitialPacket incomingPacket = clientStreamHandler.getJsonSerializer().fromJson(clientStreamHandler.getScanner().nextLine(), InitialPacket.class);
            name = incomingPacket.getName().trim();

            InitialPacket outgoingPacket = new InitialPacket();
            outgoingPacket.setName(name);

            if (name.isEmpty()) {
                outgoingPacket.setNameStatus(InitialPacket.NameStatus.NAME_EMPTY);
                clientStreamHandler.getWriter().println(clientStreamHandler.getJsonSerializer().toJson(outgoingPacket));
            } else if (clientNames.add(name)) {
                outgoingPacket.setNameStatus(InitialPacket.NameStatus.NAME_CREATED);
                clientStreamHandler.getWriter().println(clientStreamHandler.getJsonSerializer().toJson(outgoingPacket));
                clientConnectionsMap.put(name, clientStreamHandler);
                break;
            } else {
                outgoingPacket.setNameStatus(InitialPacket.NameStatus.NAME_ALREADY_PRESENTED);
                clientStreamHandler.getWriter().println(clientStreamHandler.getJsonSerializer().toJson(outgoingPacket));
            }
        }
        return name;
    }

    public void finalizeInitialization(AtomicInteger number, List<AtomicReference<MessagePacket>> chatHistory) {
        for (int i = number.get(); i < chatHistory.size() + number.get(); i++) {
            if (chatHistory.get(i % chatHistory.size()).get() != null)
                clientStreamHandler.getWriter().println(clientStreamHandler.getJsonSerializer().toJson(chatHistory.get(i % chatHistory.size()).get()));
        }
        MessagePacket messagePacketOut = new MessagePacket();
        messagePacketOut.setContent(name + " joined the chat, say hi to him! :)");

        clientStreamHandler.getWriter().println(clientStreamHandler.getJsonSerializer().toJson(messagePacketOut));
        chatHistory.get(number.getAndAdd(1) % chatHistory.size()).set(messagePacketOut);
        number.set(number.get() % chatHistory.size());
        for (String client : clientNames) {
            if (!name.equals(client)) {
                clientConnectionsMap.get(client).getWriter().println(clientConnectionsMap.get(client).getJsonSerializer().toJson(messagePacketOut));
            }
        }
    }
}


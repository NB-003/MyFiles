package chat.server.connection;

import chat.serialization.StreamHandler;
import chat.serialization.packets.MessagePacket;
import com.google.gson.Gson;

import java.io.IOException;
import java.net.Socket;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

public class ConnectionHandler implements Runnable {
    private static final int SOCKET_TIMEOUT = 10000;
    private final ConcurrentSkipListSet<String> clientNames;
    private final Socket clientSocket;
    private final ConcurrentHashMap<String, StreamHandler> clientConnectionsMap;
    private final List<AtomicReference<MessagePacket>> chatHistory;
    private final AtomicInteger chatMessageNumber;

    public ConnectionHandler(Socket clientSocket, ConcurrentHashMap<String, StreamHandler> clientConnectionsMap, ConcurrentSkipListSet<String> clientNames,
                             List<AtomicReference<MessagePacket>> chatHistory, AtomicInteger chatMessageNumber) {
        this.clientConnectionsMap = clientConnectionsMap;
        this.clientNames = clientNames;
        this.clientSocket = clientSocket;
        this.chatHistory = chatHistory;
        this.chatMessageNumber = chatMessageNumber;
    }

    @Override
    public void run() {
        try {
            StreamHandler clientStreamHandler = new StreamHandler(clientSocket, new Gson());
            clientSocket.setSoTimeout(SOCKET_TIMEOUT);

            ClientInitializer clientInitializer = new ClientInitializer(clientStreamHandler, clientNames, clientConnectionsMap);
            String clientName = clientInitializer.initialize();
            clientInitializer.finalizeInitialization(chatMessageNumber, chatHistory);

            MessageHandler messageHandler = new MessageHandler(clientStreamHandler, chatMessageNumber, clientConnectionsMap, clientNames, clientName);
            messageHandler.handle(chatHistory);
            terminateConnection(clientStreamHandler, clientName);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void terminateConnection(StreamHandler streamHandler, String clientName) {
        clientConnectionsMap.remove(clientName);
        clientNames.remove(clientName);
        String exitMessage = clientName + " left the chat. Goodbye!";
        MessagePacket messagePacket = new MessagePacket();
        messagePacket.setContent(exitMessage);
        streamHandler.getWriter().println(streamHandler.getJsonSerializer().toJson(messagePacket));
        for (String name : clientNames) {
            if (!clientName.equals(name)) {
                clientConnectionsMap.get(name).getWriter().println(clientConnectionsMap.get(name).getJsonSerializer().toJson(messagePacket));
            }
        }
        chatHistory.get(chatMessageNumber.getAndAdd(1) % chatHistory.size()).set(messagePacket);
        chatMessageNumber.set(chatMessageNumber.get() % chatHistory.size());
    }
}

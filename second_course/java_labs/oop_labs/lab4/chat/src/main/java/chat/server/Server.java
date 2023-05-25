package chat.server;

import chat.serialization.StreamHandler;
import chat.serialization.packets.MessagePacket;
import chat.server.connection.ConnectionHandler;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

public class Server {
    private static final int CHAT_HISTORY_SIZE = 10;
    private static final int PORT = 8080;

    public static void main(String[] args) throws IOException {
        ConcurrentSkipListSet<String> clientNames = new ConcurrentSkipListSet<>();
        ConcurrentHashMap<String, StreamHandler> clientConnectionsMap = new ConcurrentHashMap<>();
        ExecutorService threadPool = Executors.newCachedThreadPool();
        List<AtomicReference<MessagePacket>> chatHistory = new ArrayList<>(CHAT_HISTORY_SIZE);

        for (int i = 0; i < CHAT_HISTORY_SIZE; i++)
            chatHistory.add(new AtomicReference<>());

        AtomicInteger chatMessageNumber = new AtomicInteger(0);
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            while (true) {
                Socket clientSocket = serverSocket.accept();
                threadPool.execute(new ConnectionHandler(clientSocket, clientConnectionsMap, clientNames, chatHistory, chatMessageNumber));
            }
        }
    }
}

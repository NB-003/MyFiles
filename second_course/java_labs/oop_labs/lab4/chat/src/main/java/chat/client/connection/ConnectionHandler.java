package chat.client.connection;

import chat.serialization.StreamHandler;
import chat.serialization.packets.MessagePacket;
import javafx.application.Platform;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.scene.control.Label;

import java.net.SocketException;
import java.util.Set;

public class ConnectionHandler implements Runnable {
    private final StreamHandler streamHandler;
    private final ObservableList<String> participants;
    private final ObservableList<Label> chatMessages;
    private static final int SOCKET_TIMEOUT_MS = 1000 * 10;

    public ConnectionHandler(StreamHandler streamHandler, ObservableList<String> participants, ObservableList<Label> chatMessages) {
        this.participants = participants;
        this.chatMessages = chatMessages;
        this.streamHandler = streamHandler;
    }

    @Override
    public void run() {
        try {
            streamHandler.getSocket().setSoTimeout(SOCKET_TIMEOUT_MS);
        } catch (SocketException e) {
            e.printStackTrace();
        }
        while (streamHandler.getScanner().hasNextLine()) {
            MessagePacket incomingMessagePacket = streamHandler.getJsonSerializer().fromJson(streamHandler.getScanner().nextLine(), MessagePacket.class);

            if (incomingMessagePacket.getContent() != null) {
                String clientMessage;
                if (incomingMessagePacket.getSenderName() != null)
                    clientMessage = incomingMessagePacket.getSenderName() + " > " + incomingMessagePacket.getContent() + "\n" + incomingMessagePacket.getDate();
                else
                    clientMessage = incomingMessagePacket.getContent();
                Label messageLabel = new Label(clientMessage);

                Task<Void> task = new Task<>() {
                    @Override
                    protected Void call() {
                        Platform.runLater(() -> chatMessages.add(messageLabel));
                        return null;
                    }
                };
                new Thread(task).start();
            } else {
                Set<String> clientSet = incomingMessagePacket.getClients();

                Task<Void> task = new Task<>() {
                    @Override
                    protected Void call() {
                        for (String client : clientSet)
                            if (!participants.contains(client))
                                Platform.runLater(() -> participants.add(client));
                        Platform.runLater(() -> participants.removeIf(client -> !clientSet.contains(client)));
                        return null;
                    }
                };
                new Thread(task).start();
            }
        }
    }
}


package chat.client.gui.model;

import chat.client.connection.ConnectionHandler;
import chat.serialization.StreamHandler;
import chat.serialization.packets.MessagePacket;
import javafx.collections.ObservableList;
import javafx.scene.control.Label;

import java.io.IOException;

public class Chat {
    private final StreamHandler streamHandler;
    private final ObservableList<String> participants;
    private final ObservableList<Label> chatMessages;

    public Chat(StreamHandler streamHandler, ObservableList<String> participants, ObservableList<Label> chatMessages) {
        this.chatMessages = chatMessages;
        this.participants = participants;
        this.streamHandler = streamHandler;
    }

    public void sendMessage(MessagePacket messagePacket) throws IOException {
        streamHandler.getWriter().println(streamHandler.getJsonSerializer().toJson(messagePacket));
    }

    public void initiateChatSession() {
        new Thread(new ConnectionHandler(streamHandler, participants, chatMessages)).start();
    }
}


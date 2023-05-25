package chat.client.connection;

import chat.client.gui.model.Chat;
import chat.serialization.StreamHandler;
import chat.serialization.packets.MessagePacket;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.collections.ObservableList;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyCodeCombination;
import javafx.scene.input.KeyCombination;
import javafx.scene.input.KeyEvent;
import javafx.util.Duration;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MessageHandler {
    private static final int RESOURCE_REQUEST_INTERVAL_MS = 1000;
    private final Chat chat;

    public MessageHandler(StreamHandler streamHandler, ObservableList<String> participantNames, ObservableList<Label> chatLabels) throws IOException {
        chat = new Chat(streamHandler, participantNames, chatLabels);
        chat.initiateChatSession();
    }

    public void handleKeyPressed(KeyEvent keyEvent, TextArea messageInputArea, String userName, SimpleDateFormat dateTimeFormatter) {
        KeyCodeCombination shiftEnterCombination = new KeyCodeCombination(KeyCode.ENTER, KeyCombination.SHIFT_DOWN);
        if (shiftEnterCombination.match(keyEvent)) {
            int caretPosition = messageInputArea.getCaretPosition();
            messageInputArea.setText(messageInputArea.getText().substring(0, caretPosition) + "\n" + messageInputArea.getText().substring(caretPosition));
            messageInputArea.positionCaret(caretPosition + 1);
        } else if (KeyCode.ENTER == keyEvent.getCode()) {
            int caretPosition = messageInputArea.getCaretPosition();
            Date currentTime = new Date();
            messageInputArea.setText(messageInputArea.getText().substring(0, caretPosition - 1) + messageInputArea.getText().substring(caretPosition));

            String messageText = messageInputArea.getText().trim();
            messageInputArea.setText("");
            try {
                MessagePacket outgoingMessagePacket = new MessagePacket();
                outgoingMessagePacket.setContent(messageText);
                outgoingMessagePacket.setSenderName(userName);
                outgoingMessagePacket.setDate(dateTimeFormatter.format(currentTime));
                chat.sendMessage(outgoingMessagePacket);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void initiateResourceRequestRoutine(final String userName) {
        MessagePacket outgoingMessagePacket = new MessagePacket();
        outgoingMessagePacket.setSenderName(userName);
        Timeline timeline = new Timeline(new KeyFrame(
                Duration.millis(RESOURCE_REQUEST_INTERVAL_MS),
                ae -> {
                    try {
                        chat.sendMessage(outgoingMessagePacket);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
        ));
        timeline.setCycleCount(-1);
        timeline.play();
    }
}


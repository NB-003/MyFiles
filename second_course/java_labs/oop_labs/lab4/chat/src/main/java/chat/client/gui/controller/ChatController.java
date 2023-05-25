package chat.client.gui.controller;


import chat.client.connection.MessageHandler;
import chat.serialization.StreamHandler;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.MultipleSelectionModel;
import javafx.scene.control.TextArea;
import javafx.scene.input.Clipboard;
import javafx.scene.input.ClipboardContent;

import java.io.IOException;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.ResourceBundle;

public class ChatController implements Initializable {
    @FXML
    private ListView<Label> chatListView;
    @FXML
    private TextArea messageTextArea;
    @FXML
    private ListView<String> participantListView;

    private final StreamHandler streamHandler;
    private final String userName;

    public ChatController(StreamHandler streamHandler, String userName) {
        this.streamHandler = streamHandler;
        this.userName = userName;
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        ObservableList<String> participantObservableList = FXCollections.observableArrayList("");
        ObservableList<Label> chatObservableList = FXCollections.observableArrayList();
        participantListView.setItems(participantObservableList);
        this.chatListView.setItems(chatObservableList);

        Clipboard clipboard = Clipboard.getSystemClipboard();
        ClipboardContent clipboardContent = new ClipboardContent();
        MultipleSelectionModel<Label> chatSelectionModel = chatListView.getSelectionModel();
        MultipleSelectionModel<String> participantSelectionModel = participantListView.getSelectionModel();

        chatSelectionModel.selectedItemProperty().addListener((changed, oldValue, newValue) -> {
            clipboardContent.putString(newValue.getText());
            clipboard.setContent(clipboardContent);
        });
        participantSelectionModel.selectedItemProperty().addListener((changed, oldValue, newValue) -> {
            clipboardContent.putString(newValue);
            clipboard.setContent(clipboardContent);
        });

        try {
            MessageHandler messageHandler = new MessageHandler(streamHandler, participantObservableList, chatObservableList);
            messageTextArea.setOnKeyPressed(keyEvent -> messageHandler.handleKeyPressed(keyEvent, messageTextArea, userName,
                    new SimpleDateFormat("'The message was sent on ' dd.MM.yyyy' at' HH:mm.")));
            messageHandler.initiateResourceRequestRoutine(userName);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

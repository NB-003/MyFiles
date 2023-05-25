package chat.client.gui.model;

import chat.client.Client;
import chat.client.gui.controller.ChatController;
import chat.serialization.StreamHandler;
import chat.serialization.packets.InitialPacket;
import com.google.gson.Gson;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.text.Text;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;

public class Menu {
    public static void initiateChat(TextField userNameField, Text errorMessage, Button startButton, TextField serverAddressField,
                                    TextField serverPortField) {
        Socket socket = new Socket();
        try {
            socket.connect(new InetSocketAddress(serverAddressField.getText(), Integer.parseInt(serverPortField.getText())), 2000);

            String userName = userNameField.getText().trim();
            InitialPacket outgoingInitialPacket = new InitialPacket();
            outgoingInitialPacket.setName(userName);

            StreamHandler streamHandler = new StreamHandler(socket, new Gson());
            streamHandler.getWriter().println(streamHandler.getJsonSerializer().toJson(outgoingInitialPacket));
            InitialPacket incomingInitialPacket = streamHandler.getJsonSerializer().fromJson(streamHandler.getScanner().nextLine(), InitialPacket.class);

            if (incomingInitialPacket.getNameStatus().equals(InitialPacket.NameStatus.NAME_ALREADY_PRESENTED)) {
                errorMessage.setText("Name already exists");
            } else if (incomingInitialPacket.getNameStatus().equals(InitialPacket.NameStatus.NAME_EMPTY)) {
                errorMessage.setText("Name cannot be spaces only or empty");
            } else {
                loadChat(startButton.getScene(), streamHandler, userName);
            }
        } catch (IOException e) {
            errorMessage.setText("Could not establish a connection to the server. Please try again later or change the server IP address.");
        }
    }

    private static void loadChat(Scene scene, StreamHandler streamHandler, String userName) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(Client.class.getResource("/chat/client/gui/view/ChatView.fxml"));
        fxmlLoader.setController(new ChatController(streamHandler, userName));
        scene.setRoot(fxmlLoader.load());
    }
}

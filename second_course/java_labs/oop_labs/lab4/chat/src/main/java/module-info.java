module chat {
    requires javafx.controls;
    requires javafx.fxml;
    requires com.google.gson;


    opens chat.client to javafx.fxml;
    exports chat.client;
    exports chat.client.gui.controller;
    opens chat.client.gui.controller to javafx.fxml;

    opens chat.client.gui.model to com.google.gson;
    opens chat.server to com.google.gson;
    opens chat.serialization to com.google.gson;
    opens chat.serialization.packets to com.google.gson;
}
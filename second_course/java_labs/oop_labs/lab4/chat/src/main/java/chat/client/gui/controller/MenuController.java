package chat.client.gui.controller;

import java.net.*;
import java.util.ResourceBundle;

import chat.client.gui.model.Menu;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.text.Text;

public class MenuController implements Initializable {
    @FXML
    private TextField nameTextField;
    @FXML
    private Text errorText;
    @FXML
    private Button startButton;
    @FXML
    private TextField addressTextField;
    @FXML
    private TextField portTextField;

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        startButton.setOnAction(actionEvent -> Menu.initiateChat(nameTextField, errorText, startButton,
                addressTextField, portTextField));
    }

}

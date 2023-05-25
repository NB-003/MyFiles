package chat.client;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class Client extends Application {

    @Override
    public void start(Stage stage) throws IOException {
        Scene scene = new Scene(new FXMLLoader(getClass().getResource("/chat/client/gui/view/MenuView.fxml")).load(), 800, 600);
        stage.setScene(scene);
        stage.setTitle("Welcome to the Chat!");
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }

}

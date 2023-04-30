package tetris.view;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;

public class MenuView extends VBox {
    private Button newGameButton;
    private Button highScoresButton;
    private Button aboutButton;
    private Button exitButton;

    public MenuView() {
        setSpacing(10);
        setAlignment(Pos.CENTER);

        Label titleLabel = new Label("Tetris");
        titleLabel.setStyle("-fx-font-size: 24; -fx-font-weight: bold;");

        newGameButton = new Button("New Game");
        highScoresButton = new Button("High Scores");
        aboutButton = new Button("About");
        exitButton = new Button("Exit");

        getChildren().addAll(titleLabel, newGameButton, highScoresButton, aboutButton, exitButton);
    }

    public Button getNewGameButton() {
        return newGameButton;
    }

    public Button getHighScoresButton() {
        return highScoresButton;
    }

    public Button getAboutButton() {
        return aboutButton;
    }

    public Button getExitButton() {
        return exitButton;
    }
}

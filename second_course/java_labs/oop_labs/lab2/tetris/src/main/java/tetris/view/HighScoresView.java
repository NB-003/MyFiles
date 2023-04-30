package tetris.view;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;

public class HighScoresView extends VBox {
    private Label title;
    private Label scores;
    private Button backButton;

    public HighScoresView() {
        setSpacing(10);
        setAlignment(Pos.CENTER);

        title = new Label("High Scores");
        title.setStyle("-fx-font-size: 24; -fx-font-weight: bold;");

        scores = new Label();
        scores.setStyle("-fx-font-size: 16; -fx-font-weight: normal;");

        backButton = new Button("Back");

        getChildren().addAll(title, scores, backButton);
    }

    public void updateHighScores(String highScores) {
        scores.setText(highScores);
    }

    public Button getBackButton() {
        return backButton;
    }
}

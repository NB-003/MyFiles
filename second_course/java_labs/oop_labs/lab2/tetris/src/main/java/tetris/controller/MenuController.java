package tetris.controller;

import javafx.scene.control.Alert;
import javafx.stage.Stage;
import tetris.view.MenuView;

public class MenuController {

    private MenuView menuView;
    private GameController gameController;
    private HighScoresController highScoresController;

    public MenuController(MenuView menuView, GameController gameController, HighScoresController highScoresController) {
        this.menuView = menuView;
        this.gameController = gameController;
        this.highScoresController = highScoresController;
        initializeMenuView();
    }

    private void initializeMenuView() {
        menuView.getNewGameButton().setOnAction(event -> gameController.startGame());
        menuView.getHighScoresButton().setOnAction(event -> highScoresController.showHighScores());
        menuView.getAboutButton().setOnAction(event -> showAbout());
        menuView.getExitButton().setOnAction(event -> exitApplication());
    }

    public void showAbout() {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("About");
        alert.setHeaderText("Tetris Game");

        String content = "Creator: Bazarov Bulat\n"
            + "University and Faculty: NSU FIT\n"
            + "Group: 21201\n\n"
            + "Controls:\n"
            + "A - Move tetromino left\n"
            + "D - Move tetromino right\n"
            + "W - Rotate tetromino\n"
            + "S - Drop tetromino\n"
            + "ESCAPE - Return to main menu\n\n"
            + "Note: Tetromino is a term for Tetris shapes.";

        alert.setContentText(content);
        alert.showAndWait();
    }


    public void exitApplication() {
        Stage stage = (Stage) menuView.getScene().getWindow();
        stage.close();
    }
}

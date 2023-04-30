package tetris;

import static tetris.Constants.BOARD_HEIGHT;
import static tetris.Constants.BOARD_WIDTH;
import static tetris.Constants.TILE_SIZE;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import tetris.controller.*;
import tetris.view.GameView;
import tetris.view.HighScoresView;
import tetris.view.MenuView;

public class TetrisApp extends Application {

    private StackPane root;
    private GameView gameView;
    private HighScoresView highScoresView;
    private MenuView menuView;
    private GameController gameController;

    @Override
    public void start(Stage primaryStage) {
        root = new StackPane();

        initializeViews();
        initializeControllers();

        Scene scene = new Scene(root, BOARD_WIDTH * TILE_SIZE, BOARD_HEIGHT * TILE_SIZE);
        gameController.initializeKeyHandlers(scene);

        primaryStage.setTitle("Tetris");
        primaryStage.setScene(scene);
        primaryStage.setResizable(false);
        primaryStage.show();
    }

    private void initializeViews() {
        gameView = new GameView(BOARD_WIDTH * TILE_SIZE, BOARD_HEIGHT * TILE_SIZE);
        menuView = new MenuView();
        highScoresView = new HighScoresView();

        root.getChildren().addAll(gameView, highScoresView, menuView);
        highScoresView.setVisible(false);
        gameView.setVisible(false);
    }

    private void initializeControllers() {
        gameController = new GameController(gameView, menuView);
        new MenuController(menuView, gameController, new HighScoresController(highScoresView, menuView));
    }

    public static void main(String[] args) {
        launch(args);
    }
}

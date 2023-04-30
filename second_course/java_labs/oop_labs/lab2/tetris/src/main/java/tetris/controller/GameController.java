package tetris.controller;

import java.io.*;
import java.util.*;
import javafx.animation.AnimationTimer;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.control.*;
import tetris.model.Game;
import tetris.model.Score;
import tetris.view.GameView;
import tetris.view.MenuView;

public class GameController {
    private Game game;
    private Score score;
    private GameView gameView;
    private MenuView menuView;
    private AnimationTimer gameLoop;
    private boolean gameOverShown;

    public GameController(GameView gameView, MenuView menuView) {
        this.gameView = gameView;
        this.menuView = menuView;

        gameView.requestFocus();

        gameLoop = new AnimationTimer() {
            @Override
            public void handle(long now) {
                game.update();
                if(game.isGameOver() && !gameOverShown) {
                    gameOver();
                }
                gameView.draw();
            }
        };
    }

    public void startGame() {
        TextInputDialog dialog = new TextInputDialog("Player");
        dialog.setTitle("Enter player name");
        dialog.setHeaderText("Please enter your name:");
        dialog.setContentText("Name:");

        // Изменение типов кнопок
        dialog.getDialogPane().getButtonTypes().setAll(ButtonType.OK);

        Optional<String> playerNameResult = dialog.showAndWait();
        String playerName = playerNameResult.orElse("Player");

        score = new Score(playerName);
        game = new Game(score);
        gameView.setGame(game);

        game.start();
        gameLoop.start();
        gameView.requestFocus();
        menuView.setVisible(false);
        gameView.setVisible(true);
    }

    public void initializeKeyHandlers(Scene scene) {
        scene.setOnKeyPressed(event -> {
            switch (event.getCode()) {
                case A -> moveLeft();
                case D -> moveRight();
                case W -> rotate();
                case S -> drop();
                case ESCAPE -> showMenu();
            }
        });
    }

    private void showMenu() {
        game.stop();
        gameLoop.stop();
        gameOverShown = false;
        gameView.setVisible(false);
        menuView.setVisible(true);
    }

    private void moveLeft() {
        if (game.isRunning()) {
            game.moveLeft();
        }
    }

    private void moveRight() {
        if (game.isRunning()) {
            game.moveRight();
        }
    }

    private void rotate() {
        if (game.isRunning()) {
            game.rotate();
        }
    }

    private void drop() {
        if (game.isRunning()) {
            game.drop();
        }
    }

    private void saveScoreToFile() {
        File highScoresFile = new File("src/main/resources/high_scores.txt");

        List<Score> scores = new ArrayList<>();
        boolean nameExists = false;

        // Reading existing records from a file
        try (BufferedReader reader = new BufferedReader(new FileReader(highScoresFile))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(": ");
                String playerName = parts[0];
                int playerScore = Integer.parseInt(parts[1]);

                // If the player's name is already on the list, update the record if necessary
                if (playerName.equals(score.getPlayerName())) {
                    nameExists = true;
                    if (score.getScore() > playerScore) {
                        playerScore = score.getScore();
                    }
                }
                scores.add(new Score(playerName, playerScore));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        // If the player's name is not in the list, add it
        if (!nameExists) {
            scores.add(score);
        }

        // Sort the list of records
        scores.sort(Comparator.comparing(Score::getScore).reversed());

        // Write the sorted list back to file
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(highScoresFile, false))) {
            for (Score sortedScore : scores) {
                writer.write(sortedScore.getPlayerName() + ": " + sortedScore.getScore() + "\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void gameOver() {
        gameOverShown = true;

        saveScoreToFile();

        Platform.runLater(() -> {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Game Over");
            alert.setHeaderText("Your score: " + score.getScore());
            alert.setContentText("Press OK to return to the main menu.");

            Optional<ButtonType> result = alert.showAndWait();
            if (result.isPresent() && result.get() == ButtonType.OK) {
                // Return to Main Menu
                showMenu();
            }
        });
    }

}

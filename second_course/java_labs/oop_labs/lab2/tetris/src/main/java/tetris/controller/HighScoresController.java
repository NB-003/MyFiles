package tetris.controller;

import java.io.*;
import tetris.view.HighScoresView;
import tetris.view.MenuView;

public class HighScoresController {

    private HighScoresView highScoresView;
    private MenuView menuView;

    public HighScoresController(HighScoresView highScoresView, MenuView menuView) {
        this.highScoresView = highScoresView;
        this.menuView = menuView;
        initializeHighScoresView();
    }

    private void initializeHighScoresView() {
        highScoresView.getBackButton().setOnAction(event -> showMenu());
    }

    public void showHighScores() {
        updateHighScores();
        highScoresView.setVisible(true);
        menuView.setVisible(false);
    }

    public void showMenu() {
        highScoresView.setVisible(false);
        menuView.setVisible(true);
    }

    private void updateHighScores() {
        String highScores = readHighScoresFromFile();
        highScoresView.updateHighScores(highScores);
    }

    private String readHighScoresFromFile() {
        StringBuilder highScores = new StringBuilder();
        File highScoresFile = new File("src/main/resources/high_scores.txt");

        if (!highScoresFile.exists()) {
            return null;
        }

        try (BufferedReader reader = new BufferedReader(new FileReader(highScoresFile))) {
            String line;
            while ((line = reader.readLine()) != null) {
                highScores.append(line).append("\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return highScores.toString();
    }
}

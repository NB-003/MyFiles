package tetris.model;

public class Score {
    private String playerName;
    private int score;

    public Score(String playerName) {
        this(playerName, 0);
    }

    public Score(String playerName, int score) {
        this.playerName = playerName;
        this.score = score;
    }

    public void increaseScore(int lines) {
        score += calculatePoints(lines);
    }

    private int calculatePoints(int lines) {
        return switch (lines) {
            case 1 -> 100;
            case 2 -> 300;
            case 3 -> 500;
            case 4 -> 800;
            default -> 0;
        };
    }

    public String getPlayerName() {
        return playerName;
    }
    public int getScore() {
        return score;
    }
}
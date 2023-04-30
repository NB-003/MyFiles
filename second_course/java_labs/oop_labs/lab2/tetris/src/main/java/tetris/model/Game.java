package tetris.model;

import static tetris.Constants.MOVE_DELAY;

public class Game {
    private Board board;
    private boolean isRunning;
    private boolean isGameOver;
    private long lastMoveTime;

    public Game(Score score) {
        board = new Board(score);
        isRunning = false;
        lastMoveTime = System.currentTimeMillis();
    }

    public void start() {
        board.clearBoard();
        board.getCurrentTetromino().setRandomShape();
        isRunning = true;
    }

    public void stop() {
        isRunning = false;
    }

    public void update() {
        if (!isRunning) {
            return;
        }

        if (System.currentTimeMillis() - lastMoveTime >= MOVE_DELAY) {
            board.moveTetromino(0, 1);
            lastMoveTime = System.currentTimeMillis();
            if(board.isGameOver()) {
                stop();
                isGameOver = true;
            }
        }
    }

    public void moveLeft() {
        if (isRunning) {
            board.moveTetromino(-1, 0);
        }
    }

    public void moveRight() {
        if (isRunning) {
            board.moveTetromino(1, 0);
        }
    }

    public void rotate() {
        if (isRunning) {
            board.rotateTetromino();
        }
    }

    public void drop() {
        if (isRunning) {
            board.dropTetromino();
        }
    }

    public Board getBoard() {
        return board;
    }

    public boolean isRunning() {
        return isRunning;
    }

    public boolean isGameOver() {
        return isGameOver;
    }

}

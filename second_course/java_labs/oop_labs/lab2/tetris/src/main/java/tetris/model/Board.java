package tetris.model;

import static tetris.Constants.BOARD_HEIGHT;
import static tetris.Constants.BOARD_WIDTH;

import java.util.Random;

public class Board {
    private Score score;
    private Tetromino.Shape[][] grid;
    private Tetromino currentTetromino;

    public Board(Score score) {
        this.score = score;
        grid = new Tetromino.Shape[BOARD_HEIGHT][BOARD_WIDTH];
        clearBoard();
        currentTetromino = new Tetromino();
        currentTetromino.setRandomShape();
    }

    public void clearBoard() {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                grid[y][x] = Tetromino.Shape.EMPTY;
            }
        }
    }

    public Tetromino.Shape getShapeAt(int x, int y) {
        return grid[y][x];
    }

    public Tetromino getCurrentTetromino() {
        return currentTetromino;
    }

    public void moveTetromino(int deltaX, int deltaY) {
        int newX = currentTetromino.getX() + deltaX;
        int newY = currentTetromino.getY() + deltaY;

        if (canMove(currentTetromino, newX, newY)) {
            if (deltaX > 0) {
                currentTetromino.moveRight();
            } else if (deltaX < 0) {
                currentTetromino.moveLeft();
            }
            if (deltaY > 0) {
                currentTetromino.moveDown();
            }
        }

        if(!canMove(currentTetromino, currentTetromino.getX(), currentTetromino.getY() + 1)) {
            finishMove();
        }
    }
    public void rotateTetromino() {
        if (!canRotate(currentTetromino)) {
            return;
        }
        currentTetromino.rotateRight();
    }

    public void dropTetromino() {
        while (canMove(currentTetromino, currentTetromino.getX(), currentTetromino.getY() + 1)) {
            currentTetromino.moveDown();
        }
        finishMove();
    }

    public boolean isGameOver() {
        // If the collision is right after spawn, then the game ends.
        return !canMove(currentTetromino, currentTetromino.getX(), currentTetromino.getY());
    }

    private boolean canMove(Tetromino tetromino, int newX, int newY) {
        int[][] coordinates = tetromino.getCurrentCoordinates();

        for (int[] coordinate : coordinates) {
            int x = newX + coordinate[0];
            int y = newY + coordinate[1];

            if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
                return false;
            }

            if (grid[y][x] != Tetromino.Shape.EMPTY) {
                return false;
            }
        }

        return true;
    }

    private boolean canRotate(Tetromino tetromino) {
        tetromino.rotateRight();

        int[][] coordinates = tetromino.getCurrentCoordinates();
        for (int[] coordinate : coordinates) {
            int x = tetromino.getX() + coordinate[0];
            int y = tetromino.getY() + coordinate[1];

            if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT
                      || grid[y][x] != Tetromino.Shape.EMPTY) {
                tetromino.rotateLeft(); // Return the figure to its original position
                return false;
            }
        }

        tetromino.rotateLeft(); // Return the figure to its original position
        return true;
    }

    private void finishMove() {
        addTetrominoToGrid();
        removeFullRows();
        spawnNewTetromino();
    }

    private void addTetrominoToGrid() {
        int[][] coordinates = currentTetromino.getCurrentCoordinates();

        for (int[] coordinate : coordinates) {
            int x = currentTetromino.getX() + coordinate[0];
            int y = currentTetromino.getY() + coordinate[1];

            grid[y][x] = currentTetromino.getShape();
        }
    }

    private void removeFullRows() {
        int lines = 0;

        for (int y = BOARD_HEIGHT - 1; y >= 0; ) {
            boolean isRowFull = true;

            for (int x = 0; x < BOARD_WIDTH; x++) {
                if (grid[y][x] == Tetromino.Shape.EMPTY) {
                    isRowFull = false;
                    break;
                }
            }

            if (isRowFull) {
                lines++;

                for (int i = y; i > 0; i--) {
                    System.arraycopy(grid[i - 1], 0, grid[i], 0, BOARD_WIDTH);
                }

                for (int x = 0; x < BOARD_WIDTH; x++) {
                    grid[0][x] = Tetromino.Shape.EMPTY;
                }
            } else {
                y--;
            }
        }

        score.increaseScore(lines);
    }

    private void spawnNewTetromino() {
        currentTetromino.setRandomShape();
        int spawnX = new Random().nextInt(BOARD_WIDTH - currentTetromino.getWidth());
        int spawnY = currentTetromino.getHeight();
        currentTetromino.setX(spawnX);
        currentTetromino.setY(spawnY);
    }
}

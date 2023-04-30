package tetris.model;

import java.util.Random;

public class Tetromino {
    public enum Shape {
        EMPTY, I, O, T, S, Z, L
    }

    private Shape shape;
    private int[][] currentCoordinates;
    private int x, y;

    private static final int[][][] shapesCoordinates = new int[][][] {
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}}, // EMPTY
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, // I
        {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, // O
        {{1, 0}, {0, 0}, {2, 0}, {1, 1}}, // T
        {{1, 0}, {2, 0}, {0, 1}, {1, 1}}, // S
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}}, // Z
        {{1, 0}, {0, 0}, {2, 1}, {2, 0}}  // L
    };

    public Tetromino() {
        shape = Shape.EMPTY;
        currentCoordinates = new int[4][2];
        x = 0;
        y = 0;
    }

    public void setRandomShape() {
        shape = Shape.values()[new Random().nextInt(6) + 1];
        currentCoordinates = shapesCoordinates[shape.ordinal()];
    }

    public Shape getShape() {
        return shape;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setY(int y) {
        this.y = y;
    }

    public int[][] getCurrentCoordinates() {
        return currentCoordinates;
    }

    public void rotateRight() {
        if (shape == Shape.EMPTY || shape == Shape.O) {
            return;
        }

        int[][] newCoordinates = new int[4][2];
        for (int i = 0; i < 4; i++) {
            newCoordinates[i][0] = -currentCoordinates[i][1];
            newCoordinates[i][1] = currentCoordinates[i][0];
        }

        currentCoordinates = newCoordinates;
    }

    public void rotateLeft() {
        if (shape == Shape.EMPTY || shape == Shape.O) {
            return;
        }

        int[][] newCoordinates = new int[4][2];
        for (int i = 0; i < 4; i++) {
            newCoordinates[i][0] = currentCoordinates[i][1];
            newCoordinates[i][1] = -currentCoordinates[i][0];
        }

        currentCoordinates = newCoordinates;
    }

    public void moveLeft() {
        x -= 1;
    }

    public void moveRight() {
        x += 1;
    }

    public void moveDown() {
        y += 1;
    }

    public int getWidth() {
        int minX = currentCoordinates[0][0];
        int maxX = currentCoordinates[0][0];

        for (int i = 1; i < currentCoordinates.length; i++) {
            int x = currentCoordinates[i][0];
            if (x < minX) {
                minX = x;
            }
            if (x > maxX) {
                maxX = x;
            }
        }

        return maxX - minX;
    }

    public int getHeight() {
        int minY = currentCoordinates[0][1];
        int maxY = currentCoordinates[0][1];

        for (int i = 1; i < currentCoordinates.length; i++) {
            int y = currentCoordinates[i][1];
            if (y < minY) {
                minY = y;
            }
            if (y > maxY) {
                maxY = y;
            }
        }

        return maxY - minY;
    }
}

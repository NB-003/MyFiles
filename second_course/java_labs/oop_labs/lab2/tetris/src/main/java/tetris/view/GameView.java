package tetris.view;

import static tetris.Constants.BOARD_HEIGHT;
import static tetris.Constants.BOARD_WIDTH;
import static tetris.Constants.TILE_SIZE;

import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;
import tetris.model.*;

public class GameView extends Canvas {
    private Game game;

    public GameView(int width, int height) {
        super(width, height);
    }

    public void draw() {
        GraphicsContext gc = getGraphicsContext2D();
        Board board = game.getBoard();

        // Clear canvas
        gc.setFill(Color.BLACK);
        gc.fillRect(0, 0, getWidth(), getHeight());

        // Draw the playing field
        for (int row = 0; row < BOARD_HEIGHT; row++) {
            for (int col = 0; col < BOARD_WIDTH; col++) {
                Tetromino.Shape shape = board.getShapeAt(col, row);
                if (shape != Tetromino.Shape.EMPTY) {
                    drawTile(gc, shape, col * TILE_SIZE, row * TILE_SIZE);
                }
            }
        }

        // Draw current tetromino
        Tetromino currentTetromino = board.getCurrentTetromino();
        int[][] coordinates = currentTetromino.getCurrentCoordinates();
        for (int[] coordinate : coordinates) {
            int x = currentTetromino.getX() + coordinate[0];
            int y = currentTetromino.getY() + coordinate[1];
            drawTile(gc, currentTetromino.getShape(), x * TILE_SIZE, y * TILE_SIZE);
        }
    }

    public void setGame(Game game) {
        this.game = game;
    }

    private void drawTile(GraphicsContext gc, Tetromino.Shape shape, int x, int y) {
        Color color = getColor(shape);
        gc.setFill(color);
        gc.fillRect(x, y, TILE_SIZE, TILE_SIZE);
        gc.setStroke(color.darker());
        gc.strokeRect(x, y, TILE_SIZE, TILE_SIZE);
    }

    private Color getColor(Tetromino.Shape shape) {
        return switch (shape) {
            case I -> Color.CYAN;
            case L -> Color.ORANGE;
            case O -> Color.YELLOW;
            case S -> Color.GREEN;
            case T -> Color.PURPLE;
            case Z -> Color.RED;
            default -> Color.BLACK;
        };
    }
}

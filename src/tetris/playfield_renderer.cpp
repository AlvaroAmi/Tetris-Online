#include "tetris/playfield_renderer.hpp"
#include <iostream>

#define PLAYFIELD_WINDOW_NLINES 42
#define PLAYFIELD_WINDOW_NCOLS 42

PlayfieldRenderer::PlayfieldRenderer(int startx, int starty, const Playfield &playfield) {
    this->playfield = playfield;
    window = newwin(PLAYFIELD_WINDOW_NLINES, PLAYFIELD_WINDOW_NCOLS, starty, startx);
    box(window, 0, 0);
    wrefresh(window);
}

PlayfieldRenderer::~PlayfieldRenderer() {
    // delwin(window);
}

Color (*PlayfieldRenderer::getRenderableMatrix(const Playfield &playfield, std::optional<Tetromino> optionalTetromino))[PLAYFIELD_WIDTH] {
    Color(*renderableMatrix)[PLAYFIELD_WIDTH] = new Color[VISIBLE_PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
    // Copy playfield matrix
    for (int y = 0; y < VISIBLE_PLAYFIELD_HEIGHT; y++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            renderableMatrix[y][x] = playfield.getTile(x, y);
        }
    }

    if (!optionalTetromino.has_value()) return renderableMatrix;
    Tetromino tetromino = optionalTetromino.value();

    Vector2<int> *tetrominoBlocks = tetromino.getBlocks();
    for (int i = 0; i < BLOCKS_PER_TETROMINO; i++) {
        Vector2<int> block = tetrominoBlocks[i];
        if (block.getY() >= VISIBLE_PLAYFIELD_HEIGHT) continue;
        renderableMatrix[block.getY()][block.getX()] = tetromino.getColor();
    }

    return renderableMatrix;
}

void PlayfieldRenderer::renderPlayfield(std::optional<Tetromino> currentTetromino) const {
    Color(*matrix)[PLAYFIELD_WIDTH] = getRenderableMatrix(playfield, currentTetromino);

    for (int row = VISIBLE_PLAYFIELD_HEIGHT - 1; row >= 0; row--) {
        for (int column = 0; column < PLAYFIELD_WIDTH; column++) {
            Color color = matrix[row][column];
            int x_coordinate = 1 + column * 4;
            int y_coordinate = (VISIBLE_PLAYFIELD_HEIGHT * 2) - (1 + row * 2);

            if (color == Color::GRAY && (COLORS <= Color::GRAY || COLOR_PAIRS <= Color::GRAY)) {
                color = Color::ORANGE; // In this case we set orange (J tetromino) to be white
            }

            wattron(window, COLOR_PAIR(color));
            mvwaddstr(window, y_coordinate, x_coordinate, "    ");
            mvwaddstr(window, y_coordinate + 1, x_coordinate, "    ");
            wattroff(window, COLOR_PAIR(color));
        }
    }
    wnoutrefresh(window);
}

void PlayfieldRenderer::printPlayfield(const Playfield &playfield, const Tetromino &currentTetromino) {
    Color(*matrix)[PLAYFIELD_WIDTH] = getRenderableMatrix(playfield, currentTetromino);
    for (int y = VISIBLE_PLAYFIELD_HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            std::cout << matrix[y][x] << " ";
        }
        std::cout << std::endl;
    }
}

#include "tetris/playfield.hpp"
#include "tetris/tetromino.hpp"
#include <string.h>

Color Playfield::getTile(int x, int y) const {
    return matrix[y][x];
}

void Playfield::setTile(int x, int y, Color color) {
    matrix[y][x] = color;
}

bool Playfield::isLineFull(int line) const {
    for (int i = 0; i < PLAYFIELD_WIDTH; i++) {
        if (getTile(i, line) == 0) return false;
    }
    return true;
}

void Playfield::clearLine(int line) {
    int offsetLines = PLAYFIELD_HEIGHT - line - 1;
    memcpy(matrix[line], matrix[line + 1], sizeof(Color) * PLAYFIELD_WIDTH * offsetLines);
    memset(matrix[PLAYFIELD_HEIGHT - 1], 0, sizeof(Color) * PLAYFIELD_WIDTH);
}

int Playfield::attemptClearLines() {
    int linesCleared = 0;
    for (int i = PLAYFIELD_HEIGHT - 1; i >= 0; i--) {
        if (isLineFull(i)) {
            clearLine(i);
            linesCleared++;
        }
    }
    return linesCleared;
}

bool Playfield::isOutOfBounds(int x, int y) const {
    return x < 0 || x >= PLAYFIELD_WIDTH || y < 0 || y >= PLAYFIELD_HEIGHT;
}

bool Playfield::isTileEmpty(int x, int y) const {
    return getTile(x, y) == 0;
}

bool Playfield::tetrominoFits(const Tetromino &tetromino) const {
    Vector2<int> *blocks = tetromino.getBlocks();
    for (int i = 0; i < BLOCKS_PER_TETROMINO; i++) {
        Vector2<int> block = blocks[i];
        if (isOutOfBounds(block.getX(), block.getY()) || !isTileEmpty(block.getX(), block.getY())) {
            return false;
        }
    }
    return true;
}

void Playfield::lockTetromino(const Tetromino &tetromino) {
    Vector2<int> *blocks = tetromino.getBlocks();
    for (int i = 0; i < BLOCKS_PER_TETROMINO; i++) {
        Vector2<int> block = blocks[i];
        setTile(block.getX(), block.getY(), tetromino.getColor());
    }
}
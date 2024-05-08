#include "tetris/playfield_renderer.hpp"
#include <iostream>

Color (*PlayfieldRenderer::getRenderableMatrix(const Playfield &playfield, const Tetromino &tetromino))[PLAYFIELD_WIDTH] {
    Color(*renderableMatrix)[PLAYFIELD_WIDTH] = new Color[VISIBLE_PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
    // Copy playfield matrix
    for (int y = 0; y < VISIBLE_PLAYFIELD_HEIGHT; y++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            renderableMatrix[y][x] = playfield.getTile(x, y);
        }
    }

    Vector2<int> *tetrominoBlocks = tetromino.getBlocks();
    for (int i = 0; i < BLOCKS_PER_TETROMINO; i++) {
        Vector2<int> block = tetrominoBlocks[i];
        if (block.getY() >= VISIBLE_PLAYFIELD_HEIGHT) continue;
        renderableMatrix[block.getY()][block.getX()] = tetromino.getColor();
    }

    return renderableMatrix;
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

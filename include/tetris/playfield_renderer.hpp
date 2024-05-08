#ifndef PLAYFIELD_RENDERER_HPP
#define PLAYFIELD_RENDERER_HPP

#include "tetris/playfield.hpp"
#include "tetris/tetromino.hpp"

class PlayfieldRenderer {
public:
    static Color (*getRenderableMatrix(const Playfield &playfield, const Tetromino &tetromino))[PLAYFIELD_WIDTH];
    static void printPlayfield(const Playfield &playfield, const Tetromino &currentTetromino);
};

#endif
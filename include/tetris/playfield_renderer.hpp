#ifndef PLAYFIELD_RENDERER_HPP
#define PLAYFIELD_RENDERER_HPP

#include "tetris/playfield.hpp"
#include "tetris/tetromino.hpp"
#include <ncurses.h>

class PlayfieldRenderer {
private:
    Playfield playfield;
    WINDOW *window = nullptr;

    static Color (*getRenderableMatrix(const Playfield &playfield, const Tetromino &tetromino))[PLAYFIELD_WIDTH];

public:
    PlayfieldRenderer() = default;
    PlayfieldRenderer(int startx, int starty, const Playfield &playfield);
    ~PlayfieldRenderer();
    void renderPlayfield(const Tetromino &currentTetromino);
    static void printPlayfield(const Playfield &playfield, const Tetromino &currentTetromino);
};

#endif
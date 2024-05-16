#ifndef PLAYFIELD_RENDERER_HPP
#define PLAYFIELD_RENDERER_HPP

#include "tetris/playfield.hpp"
#include "tetris/tetromino.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

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
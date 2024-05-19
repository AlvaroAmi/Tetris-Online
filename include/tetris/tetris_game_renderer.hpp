#ifndef TETRIS_GAME_RENDERER_HPP
#define TETRIS_GAME_RENDERER_HPP

#include "playfield.hpp"
#include "tetris_game.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

class TetrisGameRenderer {
public:
    TetrisGameRenderer(int width, int height);
    ~TetrisGameRenderer();
    virtual void render() const = 0;

protected:
    void displayNextTetromino(WINDOW *win, TetrominoType tetrominoType) const;

private:
    WINDOW *outerWin;
    void initColors(void);
};

#endif // TETRIS_GAME_RENDERER_HPP
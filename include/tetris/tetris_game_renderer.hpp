#ifndef TETRIS_GAME_RENDERER_HPP
#define TETRIS_GAME_RENDERER_HPP

#include "playfield.hpp"
#include "tetris_game.hpp"

class TetrisGameRenderer {
public:
    TetrisGameRenderer();
    ~TetrisGameRenderer();
    virtual void render(const TetrisGame &game) = 0;

private:
    WINDOW *outerWin;
    void initColors(void);
};

#endif // TETRIS_GAME_RENDERER_HPP
#ifndef SINGLEPLAYER_TETRIS_GAME_RENDERER_HPP
#define SINGLEPLAYER_TETRIS_GAME_RENDERER_HPP

#include "tetris/tetris_game_renderer.hpp"

class SingleplayerTetrisGameRenderer : TetrisGameRenderer {
private:
public:
    SingleplayerTetrisGameRenderer();
    ~SingleplayerTetrisGameRenderer();
    void render(const TetrisGame &game) override;
};

#endif // SINGLEPLAYER_TETRIS_GAME_RENDERER_HPP
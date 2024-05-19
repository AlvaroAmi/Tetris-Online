#ifndef SINGLEPLAYER_TETRIS_GAME_RENDERER_HPP
#define SINGLEPLAYER_TETRIS_GAME_RENDERER_HPP

#include "tetris/playfield_renderer.hpp"
#include "tetris/singleplayer_tetris_game.hpp"
#include "tetris/tetris_game_renderer.hpp"

class SingleplayerTetrisGameRenderer : TetrisGameRenderer {
private:
    WINDOW *titleWin;

    // Data windows: Score, Level, Lines
    WINDOW *scoreWin;
    WINDOW *levelWin;
    WINDOW *linesWin;

    WINDOW *nextWin;
    WINDOW *nextDisplayWin;

    PlayfieldRenderer playfieldRenderer;
    const SingleplayerTetrisGame &game;

public:
    SingleplayerTetrisGameRenderer(const SingleplayerTetrisGame &game);
    ~SingleplayerTetrisGameRenderer();
    void render() const override;
};

#endif // SINGLEPLAYER_TETRIS_GAME_RENDERER_HPP
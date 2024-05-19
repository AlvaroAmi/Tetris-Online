#ifndef MULTIPLAYER_TETRIS_GAME_RENDERER_HPP
#define MULTIPLAYER_TETRIS_GAME_RENDERER_HPP

#include "tetris/multiplayer_tetris_game.hpp"
#include "tetris/tetris_game_renderer.hpp"

class MultiplayerTetrisGameRenderer : public TetrisGameRenderer {
private:
    WINDOW *title_win;
    WINDOW *player1_username_win;
    WINDOW *player2_username_win;
    WINDOW *player1_garbage_win;
    WINDOW *player2_garbage_win;
    WINDOW *player1_next_win;
    WINDOW *player2_next_win;
    WINDOW *player1_next_display_win;
    WINDOW *player2_next_display_win;

    const MultiplayerTetrisGame &game;
    PlayfieldRenderer playfieldRenderer;
    PlayfieldRenderer enemyPlayfieldRenderer;

public:
    MultiplayerTetrisGameRenderer(const MultiplayerTetrisGame &game);
    ~MultiplayerTetrisGameRenderer();
    void render() const override;
};

#endif // MULTIPLAYER_TETRIS_GAME_RENDERER_HPP

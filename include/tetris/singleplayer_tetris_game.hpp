#ifndef SINGLEPLAYER_TETRIS_GAME_HPP
#define SINGLEPLAYER_TETRIS_GAME_HPP

#include "tetris/bag_random_generator.hpp"
#include "tetris/playfield.hpp"
#include "tetris/playfield_renderer.hpp"
#include "tetris/srs_manager.hpp"
#include "tetris/tetris_game.hpp"
#include "tetris/tetromino.hpp"

class SingleplayerTetrisGameRenderer;

class SingleplayerTetrisGame : TetrisGame {
    friend class SingleplayerTetrisGameRenderer;
    static constexpr int GRAVITY[MAX_DEFINED_LEVEL + 1] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};

    const SingleplayerTetrisGameRenderer &renderer;
    unsigned int score = 0;
    int totalLinesCleared = 0;

public:
    SingleplayerTetrisGame();

private:
    unsigned int getLevel() const;

    virtual void lockTetromino() override;

    int calculateScore(int linesCleared) const;

    void lineClearCallback(int linesCleared) override;

    void resetTicksTillGravity() override;

    void gameOver() override;
};

#endif // SINGLEPLAYER_TETRIS_GAME_HPP
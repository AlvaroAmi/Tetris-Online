#ifndef MULTIPLAYER_TETRIS_GAME_HPP
#define MULTIPLAYER_TETRIS_GAME_HPP

#include "tetris/bag_random_generator.hpp"
#include "tetris/playfield.hpp"
#include "tetris/playfield_renderer.hpp"
#include "tetris/srs_manager.hpp"
#include "tetris/tetris_game.hpp"
#include "tetris/tetromino.hpp"
#include <winsock2.h>

class MultiplayerTetrisGameRenderer;

class MultiplayerTetrisGame : TetrisGame {
    friend class MultiplayerTetrisGameRenderer;
    const MultiplayerTetrisGameRenderer &renderer;
    SOCKET sock;

    int pendingGarbageLines = 0;

    int garbageLinesSent = 0;
    int garbageLinesReceived = 0;

    Playfield enemyPlayfield;

public:
    MultiplayerTetrisGame(SOCKET sock);

    void enqueueGarbage(int lines);

    void updateEnemyPlayfield(std::string &matrixString);

private:
    virtual void lockTetromino() override;

    void sendPlayfield();

    void addGarbage();

    void sendGarbage(int lines);

    void lineClearCallback(int linesCleared) override;

    void resetTicksTillGravity() override;

    void gameOver() override;
};

#endif // SINGLEPLAYER_TETRIS_GAME_HPP
#include "tetris/multiplayer_tetris_game.hpp"
#include "client.hpp"
#include "tetris/multiplayer_tetris_game_renderer.hpp"
#include <chrono>
#include <random>
#include <thread>

#define GRAVITY 33

MultiplayerTetrisGame::MultiplayerTetrisGame(SOCKET sock)
    : sock(sock), renderer(*this), gen(std::random_device()()), distribution(0, PLAYFIELD_WIDTH - 1) {
    nextTetrominoType = bagRandomGenerator.getNextTetrominoType();
    spawnTetromino();
    resetTicksTillGravity();
}

void MultiplayerTetrisGame::startGame() {
    while (!gameIsOver) {
        updateGame();

        if (ticksTillGravity-- <= 0) {
            tickGravity();
        }
        sendPlayfield();
        renderer.render();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    endwin();
}

void MultiplayerTetrisGame::lockTetromino() {
    TetrisGame::lockTetromino();
    addGarbage(); // Add garbage after the tetromino is locked
    spawnTetromino();
}

void MultiplayerTetrisGame::sendPlayfield() {
    Color(*visibleMatrix)[PLAYFIELD_WIDTH] = PlayfieldRenderer::getRenderableMatrix(playfield, currentTetromino);

    std::string matrixString = "";
    for (int y = 0; y < VISIBLE_PLAYFIELD_HEIGHT; y++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            matrixString += std::to_string(visibleMatrix[y][x]);
        }
    }

    send_game_update(sock, matrixString);
}

void MultiplayerTetrisGame::updateEnemyPlayfield(std::string &matrixString) {
    for (int y = 0; y < VISIBLE_PLAYFIELD_HEIGHT; y++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            Color cellColor = (Color)(matrixString.at(y * PLAYFIELD_WIDTH + x) - '0');
            enemyPlayfield.setTile(x, y, cellColor);
        }
    }
}

void MultiplayerTetrisGame::enqueueGarbage(int lines) {
    pendingGarbageLines += lines;
}

void MultiplayerTetrisGame::addGarbage() {
    if (pendingGarbageLines == 0) return;

    garbageLinesReceived += pendingGarbageLines;
    int garbageHoleIndex = distribution(gen);

    // Shift the playfield up
    for (int y = PLAYFIELD_HEIGHT - pendingGarbageLines; y >= 0; y--) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            playfield.setTile(x, y + pendingGarbageLines, playfield.getTile(x, y));
        }
    }

    // Add the garbage
    for (int i = 0; i < pendingGarbageLines; i++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            if (x == garbageHoleIndex)
                playfield.setTile(x, i, Color::NONE);
            else
                playfield.setTile(x, i, Color::GRAY);
        }
    }

    pendingGarbageLines = 0;
}

void MultiplayerTetrisGame::sendGarbage(int lines) {
    garbageLinesSent += lines;
    send_garbage(sock, lines);
}

void MultiplayerTetrisGame::lineClearCallback(int linesCleared) {
    if (linesCleared == 0) return;
    if (linesCleared == 4)
        this->sendGarbage(4);
    else
        this->sendGarbage(linesCleared - 1);
}

void MultiplayerTetrisGame::resetTicksTillGravity() {
    ticksTillGravity = GRAVITY;
}

void MultiplayerTetrisGame::gameOver() {
    gameIsOver = true;
    send_game_finish(sock);

    endTime = std::chrono::system_clock::now();

    std::time_t start_time_t = std::chrono::system_clock::to_time_t(startTime);
    std::time_t end_time_t = std::chrono::system_clock::to_time_t(endTime);

    char start_str[20];
    char end_str[20];
    std::strftime(start_str, sizeof(start_str), "%Y-%m-%d %H:%M:%S", std::localtime(&start_time_t));
    std::strftime(end_str, sizeof(end_str), "%Y-%m-%d %H:%M:%S", std::localtime(&end_time_t));

    send_multiplayer_game_finish(sock, 0, 0, false, start_str, end_str);
}

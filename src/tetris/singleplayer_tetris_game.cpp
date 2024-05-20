#include "tetris/singleplayer_tetris_game.hpp"
#include "client.hpp"
#include "database.h"
#include "tetris/playfield_renderer.hpp"
#include "tetris/singleplayer_tetris_game.hpp"
#include "tetris/singleplayer_tetris_game_renderer.hpp"
#include "tetris/srs_manager.hpp"
#include "tetris/tetris_game.hpp"
#include "tetris/tetromino.hpp"
#include <chrono>
#include <iostream>
#include <thread>

SingleplayerTetrisGame::SingleplayerTetrisGame(SOCKET sock) : sock(sock), renderer(*this) {
    nextTetrominoType = bagRandomGenerator.getNextTetrominoType();
    spawnTetromino();
    resetTicksTillGravity();

    while (!gameIsOver) {
        updateGame();

        if (ticksTillGravity-- <= 0) tickGravity();
        renderer.render();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    endwin();
    delete this;
}

void SingleplayerTetrisGame::gameOver() {
    gameIsOver = true;
    // TODO: Implement game over screen
    endTime = std::chrono::system_clock::now();

    std::time_t start_time_t = std::chrono::system_clock::to_time_t(startTime);
    std::time_t end_time_t = std::chrono::system_clock::to_time_t(endTime);

    char start_str[20];
    char end_str[20];
    std::strftime(start_str, sizeof(start_str), "%Y-%m-%d %H:%M:%S", std::localtime(&start_time_t));
    std::strftime(end_str, sizeof(end_str), "%Y-%m-%d %H:%M:%S", std::localtime(&end_time_t));

    send_singleplayer_game_finish(sock, start_str, end_str, score, totalLinesCleared, getLevel());
}

unsigned int SingleplayerTetrisGame::getLevel() const {
    return totalLinesCleared / 10;
}

void SingleplayerTetrisGame::lockTetromino() {
    TetrisGame::lockTetromino();
    spawnTetromino();
}

int SingleplayerTetrisGame::calculateScore(int linesCleared) const {
    int level = getLevel();

    switch (linesCleared) {
    case 0: return 0;
    case 1: return 40 * (level + 1);
    case 2: return 100 * (level + 1);
    case 3: return 300 * (level + 1);
    case 4: return 1200 * (level + 1);
    }
}

void SingleplayerTetrisGame::lineClearCallback(int linesCleared) {
    totalLinesCleared += linesCleared;
    score += calculateScore(linesCleared);
}

void SingleplayerTetrisGame::resetTicksTillGravity() {
    ticksTillGravity = GRAVITY[std::min(getLevel(), (unsigned int)MAX_DEFINED_LEVEL)];
}
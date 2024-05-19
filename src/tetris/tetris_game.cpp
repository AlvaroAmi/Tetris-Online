#include "tetris/tetris_game.hpp"
#include "tetris/bag_random_generator.hpp"
#include "tetris/playfield.hpp"
#include "tetris/srs_manager.hpp"
#include "tetris/tetromino.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

TetrisGame::TetrisGame(){
    startTime = std::chrono::system_clock::now();
}

void TetrisGame::updateGame() {
    switch (getch()) {
    case KEY_LEFT:
        tryMove(LEFT_VECTOR);
        break;
    case KEY_RIGHT:
        tryMove(RIGHT_VECTOR);
        break;
    case KEY_DOWN:
        tryMove(DOWN_VECTOR);
        break;
    case ' ':
        while (playfield.tetrominoFits(currentTetromino)) {
            currentTetromino.setPosition(currentTetromino.getPosition() + DOWN_VECTOR);
        }
        currentTetromino.setPosition(currentTetromino.getPosition() - DOWN_VECTOR);
        lockTetromino();
        break;
    case 'x':
        SRSManager::rotateTetromino(currentTetromino, RotationDirection::CLOCKWISE, playfield);
        break;
    case 'z':
        SRSManager::rotateTetromino(currentTetromino, RotationDirection::COUNTERCLOCKWISE, playfield);
        break;
    }
}

void TetrisGame::spawnTetromino() {
    currentTetromino = Tetromino(nextTetrominoType, BASE_SPAWN_POSITION);
    nextTetrominoType = bagRandomGenerator.getNextTetrominoType();

    if (playfield.tetrominoFits(currentTetromino)) return;

    // Tetromino does not fit, try to spawn it one row higher
    currentTetromino.setPosition(BASE_SPAWN_POSITION + Vector2<int>(0, 1));
    if (!playfield.tetrominoFits(currentTetromino)) gameOver();
};

void TetrisGame::tryMove(Vector2<int> direction) {
    currentTetromino.setPosition(currentTetromino.getPosition() + direction);
    if (!playfield.tetrominoFits(currentTetromino)) {
        currentTetromino.setPosition(currentTetromino.getPosition() - direction);
    }
}

void TetrisGame::lockTetromino() {
    playfield.lockTetromino(currentTetromino);

    int linesCleared = playfield.attemptClearLines();
    lineClearCallback(linesCleared);
}

void TetrisGame::tickGravity() {
    currentTetromino.setPosition(currentTetromino.getPosition() + DOWN_VECTOR);

    if (!playfield.tetrominoFits(currentTetromino)) {
        currentTetromino.setPosition(currentTetromino.getPosition() - DOWN_VECTOR);
        lockTetromino();
    }

    resetTicksTillGravity();
}
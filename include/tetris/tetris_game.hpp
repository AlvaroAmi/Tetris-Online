#ifndef TETRIS_GAME_HPP
#define TETRIS_GAME_HPP

#include "tetris/bag_random_generator.hpp"
#include "tetris/playfield.hpp"
#include "tetris/vector2.hpp"
#include <chrono>

#define MAX_DEFINED_LEVEL 29

class TetrisGame {
protected:
    static constexpr Vector2<int> BASE_SPAWN_POSITION = Vector2<int>(3, 19);
    static constexpr Vector2<int> LEFT_VECTOR = Vector2<int>(-1, 0);
    static constexpr Vector2<int> RIGHT_VECTOR = Vector2<int>(1, 0);
    static constexpr Vector2<int> DOWN_VECTOR = Vector2<int>(0, -1);

    Playfield playfield;
    Tetromino currentTetromino;

    TetrominoType nextTetrominoType; // Buffered tetromino type that appears in the "next" window
    BagRandomGenerator bagRandomGenerator;

    std::chrono::time_point<std::chrono::system_clock> startTime;
    std::chrono::time_point<std::chrono::system_clock> endTime;

    bool gameIsOver = false;
    int ticksTillGravity; // NOTE: Initialize this in the constructor of the derived class to the base gravity value

    TetrisGame();
    void spawnTetromino();

    void tryMove(Vector2<int> direction);

    virtual void lockTetromino();

    void tickGravity();

    virtual void updateGame();
    virtual void resetTicksTillGravity() = 0;
    virtual void lineClearCallback(int linesCleared) = 0;
    virtual void gameOver() = 0;
};

#endif // TETRIS_GAME_HPP
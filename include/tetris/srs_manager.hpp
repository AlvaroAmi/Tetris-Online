#ifndef SRS_MANAGER_HPP
#define SRS_MANAGER_HPP

#include "tetris/playfield.hpp"
#include "tetris/tetromino.hpp"

#define NUM_SRS_TESTS 5

class SRSManager {
private:
    static const Vector2<int> (*getOffsetTable(TetrominoType type))[NUM_SRS_TESTS];

public:
    static void rotateTetromino(Tetromino &tetromino, RotationDirection rotationDirection, const Playfield &playfield);
};

#endif
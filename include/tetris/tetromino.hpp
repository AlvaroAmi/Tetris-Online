#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "tetris/vector2.hpp"

#define NUM_TETROMINOS 7
#define NUM_ROTATIONS 4 // Rotations are represented as 0 (default/spawn rotation), 1, 2, 3
#define BLOCKS_PER_TETROMINO 4

enum TetrominoType {
    I,
    J,
    L,
    O,
    S,
    T,
    Z
};

extern Vector2<int> TETROMINOS[NUM_TETROMINOS][NUM_ROTATIONS][BLOCKS_PER_TETROMINO];

enum RotationDirection {
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
};

enum Color {
    NONE,
    CYAN,
    BLUE,
    ORANGE,
    YELLOW,
    GREEN,
    PURPLE,
    RED,
    GRAY // For garbage blocks
};

class Tetromino {
private:
    TetrominoType type;
    Vector2<int> position;
    unsigned short rotation;

public:
    Tetromino() = default;
    Tetromino(TetrominoType type, Vector2<int> position);
    Tetromino(TetrominoType type, Vector2<int> position, unsigned short rotation);

    void setPosition(Vector2<int> position);
    void setRotation(unsigned short rotation);

    Vector2<int> *getBlocks() const;
    Vector2<int> getPosition() const;

    unsigned short getRotation() const;
    TetrominoType getType() const;
    Color getColor() const;
};

#endif // TETROMINO_HPP
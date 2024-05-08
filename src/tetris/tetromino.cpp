#include "tetris/tetromino.hpp"

// Tetromino coordinates depending on the rotation. The rotation 0 is adjusted to have the piece "laying on the ground" and stuck to the left.
// This is done to simplify the spawning point of the tetrominos to the same coordinate for all.
Vector2<int> TETROMINOS[NUM_TETROMINOS][NUM_ROTATIONS][BLOCKS_PER_TETROMINO] = {
    // I
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
     {{2, -2}, {2, -1}, {2, 0}, {2, 1}},
     {{0, -1}, {1, -1}, {2, -1}, {3, -1}},
     {{1, -2}, {1, -1}, {1, 0}, {1, 1}}},
    // J,
    {{{0, 1}, {0, 0}, {1, 0}, {2, 0}},
     {{2, 1}, {1, -1}, {1, 0}, {1, 1}},
     {{2, -1}, {0, 0}, {1, 0}, {2, 0}},
     {{0, -1}, {1, -1}, {1, 0}, {1, 1}}},
    // L,
    {{{2, 1}, {0, 0}, {1, 0}, {2, 0}},
     {{2, -1}, {1, -1}, {1, 0}, {1, 1}},
     {{0, -1}, {0, 0}, {1, 0}, {2, 0}},
     {{0, 1}, {1, -1}, {1, 0}, {1, 1}}},
    // O,
    {{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
     {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
     {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
     {{1, 0}, {2, 0}, {1, 1}, {2, 1}}},
    // S,
    {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
     {{1, 1}, {1, 0}, {2, 0}, {2, -1}},
     {{0, -1}, {0, 0}, {1, 0}, {1, 1}},
     {{0, 1}, {0, 0}, {1, 0}, {1, -1}}},
    // T,
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}},
     {{1, -1}, {1, 0}, {1, 1}, {2, 0}},
     {{0, 0}, {1, 0}, {2, 0}, {1, -1}},
     {{1, -1}, {1, 0}, {1, 1}, {0, 0}}},
    // Z,
    {{{0, 1}, {1, 1}, {1, 0}, {2, 0}},
     {{1, -1}, {1, 0}, {2, 0}, {2, 1}},
     {{0, 0}, {1, 0}, {1, -1}, {2, -1}},
     {{0, -1}, {0, 0}, {1, 0}, {1, 1}}},
};

Tetromino::Tetromino(TetrominoType type, Vector2<int> position) : type(type), position(position), rotation(0) {}
Tetromino::Tetromino(TetrominoType type, Vector2<int> position, unsigned short rotation) : type(type), position(position), rotation(rotation) {}

void Tetromino::setPosition(Vector2<int> position) {
    this->position = position;
}

void Tetromino::setRotation(unsigned short rotation) {
    this->rotation = rotation;
}

Vector2<int> Tetromino::getPosition() const {
    return position;
}

unsigned short Tetromino::getRotation() const {
    return rotation;
}

TetrominoType Tetromino::getType() const {
    return type;
}

Color Tetromino::getColor() const {
    switch (this->getType()) {
    case TetrominoType::I: return Color::CYAN;
    case TetrominoType::J: return Color::BLUE;
    case TetrominoType::L: return Color::ORANGE;
    case TetrominoType::O: return Color::YELLOW;
    case TetrominoType::S: return Color::GREEN;
    case TetrominoType::T: return Color::PURPLE;
    case TetrominoType::Z: return Color::RED;
    }
};

Vector2<int> *Tetromino::getBlocks() const {
    const Vector2<int> *blocks = TETROMINOS[this->getType()][this->getRotation()];
    Vector2<int> *copiedBlocks = new Vector2<int>[BLOCKS_PER_TETROMINO];
    for (int i = 0; i < BLOCKS_PER_TETROMINO; i++) {
        copiedBlocks[i] = blocks[i] + this->getPosition();
    }

    return copiedBlocks;
}
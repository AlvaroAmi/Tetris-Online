#include "tetris/srs_manager.hpp"
#include "tetris/tetromino.hpp"
#include "tetris/vector2.hpp"

#define NUM_SRS_TESTS 5

// The real offsets are calculated by subtracting the offset of the target rotation from the original rotation: offset = original - target
const Vector2<int> SRS_OFFSET_DATA_JLTSZ[NUM_ROTATIONS][NUM_SRS_TESTS] = {
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
};

const Vector2<int> SRS_OFFSET_DATA_I[NUM_ROTATIONS][NUM_SRS_TESTS] = {
    {{0, 0}, {-1, 0}, {2, 0}, {-1, 0}, {2, 0}},
    {{0, 0}, {1, 0}, {1, 0}, {1, 1}, {1, -2}},
    {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, -1}},
    {{0, 0}, {0, 0}, {0, 0}, {0, -2}, {0, 1}},
};

const Vector2<int> (*SRSManager::getOffsetTable(TetrominoType type))[NUM_SRS_TESTS] {
    if (type == TetrominoType::I) {
        return SRS_OFFSET_DATA_I;
    }
    return SRS_OFFSET_DATA_JLTSZ;
}

void SRSManager::rotateTetromino(Tetromino &tetromino, RotationDirection rotationDirection, const Playfield &playfield) {
    unsigned short newRotation = (tetromino.getRotation() + rotationDirection + NUM_ROTATIONS) % NUM_ROTATIONS;
    const Vector2<int>(*offsetData)[NUM_SRS_TESTS] = getOffsetTable(tetromino.getType());

    for (int i = 0; i < NUM_SRS_TESTS; i++) {
        const Vector2<int> offset = offsetData[tetromino.getRotation()][i] - offsetData[newRotation][i];
        Vector2<int> newPosition = tetromino.getPosition() + offset;

        if (playfield.tetrominoFits({tetromino.getType(), newPosition, newRotation})) {
            tetromino.setRotation(newRotation);
            tetromino.setPosition(newPosition);
            return;
        }
    }
}
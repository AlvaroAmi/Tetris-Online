#include "tetris/playfield.hpp"
#include "tetris/playfield_renderer.hpp"
#include "tetris/srs_manager.hpp"
#include "tetris/tetromino.hpp"
#include "tetris/vector2.hpp"

void testLineClear() {
    Playfield playfield;
    Tetromino tetromino1(TetrominoType::I, Vector2<int>(0, 0));
    Tetromino tetromino2(TetrominoType::I, Vector2<int>(4, 0));
    Tetromino tetromino3(TetrominoType::O, Vector2<int>(7, 0));
    Tetromino tetromino4(TetrominoType::T, Vector2<int>(2, 3));
    Tetromino tetrominoNotLocked(TetrominoType::O, Vector2<int>(4, 5));

    playfield.lockTetromino(tetromino1);
    playfield.lockTetromino(tetromino2);
    playfield.lockTetromino(tetromino3);
    playfield.lockTetromino(tetromino4);

    tetromino1.setPosition(tetromino1.getPosition() + Vector2<int>(0, 1));
    tetromino2.setPosition(tetromino2.getPosition() + Vector2<int>(0, 1));
    playfield.lockTetromino(tetromino1);
    playfield.lockTetromino(tetromino2);

    int i = playfield.attemptClearLines();

    PlayfieldRenderer::printPlayfield(playfield, tetrominoNotLocked);

    std::cout << "----------\n"
              << i << std::endl;
}

void testSRS() {
    Playfield playfield;
    Tetromino tetromino1(TetrominoType::I, Vector2<int>(0, 0));
    Tetromino tetromino2(TetrominoType::Z, Vector2<int>(3, 4));

    SRSManager::rotateTetromino(tetromino1, RotationDirection::COUNTERCLOCKWISE, playfield);
    playfield.lockTetromino(tetromino2);

    PlayfieldRenderer::printPlayfield(playfield, tetromino1);
}

int main(void) {
    // testSRS();
    testLineClear();

    return 0;
}
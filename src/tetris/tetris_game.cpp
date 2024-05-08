#include "tetris/bag_random_generator.hpp"
#include "tetris/playfield.hpp"
#include "tetris/tetromino.hpp"

class TetrisGame {
private:
    static constexpr Vector2<int> BASE_SPAWN_POSITION = Vector2<int>(4, 20);
    static constexpr Vector2<int> LEFT = Vector2<int>(-1, 0);
    static constexpr Vector2<int> RIGHT = Vector2<int>(1, 0);
    static constexpr Vector2<int> DOWN = Vector2<int>(0, -1);

    Playfield playfield;
    Tetromino currentTetromino;
    Tetromino nextTetromino;
    BagRandomGenerator bagRandomGenerator;

    void spawnTetromino() {
        TetrominoType tetrominoType = bagRandomGenerator.getNextTetrominoType();
        currentTetromino = Tetromino(tetrominoType, BASE_SPAWN_POSITION);

        if (playfield.tetrominoFits(currentTetromino)) return;

        // Tetromino does not fit, try to spawn it one row higher
        currentTetromino.setPosition(BASE_SPAWN_POSITION + Vector2<int>(0, 1));
        if (!playfield.tetrominoFits(currentTetromino)) gameOver();
    };

    int calculateScore(int linesCleared, int level) {
        switch (linesCleared) {
        case 1: return 40 * (level + 1);
        case 2: return 100 * (level + 1);
        case 3: return 300 * (level + 1);
        case 4: return 1200 * (level + 1);
        }
    }

    void gameOver() {
        std::cout << "Game Over!" << std::endl;
    }

public:
    void updateGame() {}
};
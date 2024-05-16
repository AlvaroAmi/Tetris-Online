#ifndef SINGLEPLAYER_TETRIS_GAME_HPP
#define SINGLEPLAYER_TETRIS_GAME_HPP

#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

#include "tetris/bag_random_generator.hpp"
#include "tetris/playfield.hpp"
#include "tetris/playfield_renderer.hpp"
#include "tetris/srs_manager.hpp"
#include "tetris/tetris_game.hpp"
#include "tetris/tetromino.hpp"

#define CUSTOM_RED 16
#define CUSTOM_GREEN 17
#define CUSTOM_YELLOW 18
#define CUSTOM_BLUE 19
#define CUSTOM_CYAN 20
#define CUSTOM_ORANGE 21

#define WIN_HEIGHT 46
#define WIN_WIDTH 102

class SingleplayerTetrisGame : TetrisGame {
    Playfield playfield;
    Tetromino currentTetromino;

    TetrominoType nextTetrominoType; // Buffered tetromino type that appears in the "next" window
    BagRandomGenerator bagRandomGenerator;

    // Windows
    WINDOW *outer_win;
    PlayfieldRenderer playfieldRenderer;

    bool gameIsOver = false;
    unsigned int score = 0;
    int totalLinesCleared = 0;

    int ticksTillGravity = GRAVITY[getLevel()];

public:
    SingleplayerTetrisGame();

private:
    void init_colors(void);

    void updateGame();

    void gameOver();

    unsigned int getLevel();

    void spawnTetromino();

    void tryMove(Vector2<int> direction);

    int calculateScore(int linesCleared);

    void lockTetromino();

    void tickGravity();

    void resetTicksTillGravity();
};

#endif // SINGLEPLAYER_TETRIS_GAME_HPP
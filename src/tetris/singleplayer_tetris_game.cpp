#include "tetris/singleplayer_tetris_game.hpp"
#include "tetris/bag_random_generator.hpp"
#include "tetris/playfield.hpp"
#include "tetris/playfield_renderer.hpp"
#include "tetris/srs_manager.hpp"
#include "tetris/tetris_game.hpp"
#include "tetris/tetromino.hpp"
#include <chrono>
#include <iostream>
#include <thread>

#define CUSTOM_RED 16
#define CUSTOM_GREEN 17
#define CUSTOM_YELLOW 18
#define CUSTOM_BLUE 19
#define CUSTOM_CYAN 20
#define CUSTOM_ORANGE 21

#define WIN_HEIGHT 46
#define WIN_WIDTH 102

SingleplayerTetrisGame::SingleplayerTetrisGame() : playfield() {
    nextTetrominoType = bagRandomGenerator.getNextTetrominoType();
    spawnTetromino();

    initscr();
    if (LINES < WIN_HEIGHT || COLS < WIN_WIDTH) {
        endwin();
        std::cout << "Terminal window too small!" << std::endl;
        exit(1);
    }

    cbreak();              // Don't wait for Enter key
    noecho();              // Don't echo any keypresses
    keypad(stdscr, TRUE);  // Allow for arrow key input
    nodelay(stdscr, TRUE); // Don't block on getch()
    curs_set(0);           // Hide cursor

    init_colors();

    outer_win = newwin(WIN_HEIGHT, WIN_WIDTH, 0, 0);
    WINDOW *title_win = newwin(10, 50, 4, 50);

    // Data windows: Score, Level, Lines
    WINDOW *score_win = newwin(3, 46, 16, 51);
    WINDOW *level_win = newwin(3, 46, 20, 51);
    WINDOW *lines_win = newwin(3, 46, 24, 51);

    WINDOW *next_win = newwin(14, 46, 29, 50);
    WINDOW *next_display_win = newwin(9, 18, 32, 65);

    refresh();

    mvwprintw(title_win, 0, 0, "  ////////////////////////////////////////////  \n"
                               "  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  \n"
                               "        _____ _____ _____ ____  ___ ____        \n"
                               "       |_   _| ____|_   _|  _ \\|_ _/ ___|       \n"
                               "         | | |  _|   | | | |_) || |\\___ \\      \n"
                               "         | | | |___  | | |  _ < | | ___)|        \n"
                               "         |_| |_____| |_| |_| \\_\\___|____/      \n"
                               "                                                 \n"
                               "  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  \n"
                               "  ////////////////////////////////////////////  \n");

    wattron(score_win, A_BOLD);
    wattron(level_win, A_BOLD);
    wattron(lines_win, A_BOLD);

    mvwprintw(score_win, 1, 5, "SCORE: \t\t\t 0");
    mvwprintw(level_win, 1, 5, "LEVEL: \t\t\t 0");
    mvwprintw(lines_win, 1, 5, "LINES: \t\t\t 0");

    mvwprintw(next_win, 0, 0, " /////////////////////////////////////////////  \n"
                              "    \t\tNEXT TETROMINO:                           \n");

    mvwprintw(next_win, 13, 0, " \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");

    box(outer_win, 0, 0);
    box(score_win, 0, 0);
    box(level_win, 0, 0);
    box(lines_win, 0, 0);
    box(next_display_win, 0, 0);

    wrefresh(outer_win);
    wrefresh(title_win);
    wrefresh(score_win);
    wrefresh(level_win);
    wrefresh(lines_win);
    wrefresh(next_win);
    wrefresh(next_display_win);
    wrefresh(outer_win);

    playfieldRenderer = PlayfieldRenderer(4, 2, playfield);
    playfieldRenderer.renderPlayfield(currentTetromino);

    while (!gameIsOver) {
        updateGame();

        if (ticksTillGravity-- <= 0) tickGravity();

        playfieldRenderer.renderPlayfield(currentTetromino);
        mvwprintw(score_win, 1, 5, "SCORE: \t\t\t %d", score);
        mvwprintw(level_win, 1, 5, "LEVEL: \t\t\t %d", getLevel());
        mvwprintw(lines_win, 1, 5, "LINES: \t\t\t %d", totalLinesCleared);

        Vector2<int> *nextTetrominoBlocks = TETROMINOS[nextTetrominoType][0];
        wattron(next_display_win, COLOR_PAIR(nextTetrominoType + 1));
        for (int i = 0; i < BLOCKS_PER_TETROMINO; i++) {
            Vector2<int> block = nextTetrominoBlocks[i];

            mvwaddstr(next_display_win, (block.getY() + 1) * 2, block.getX() * 4 + 3, "    ");
            mvwaddstr(next_display_win, (block.getY() + 1) * 2 + 1, block.getX() * 4 + 3, "    ");
        }
        wattroff(next_display_win, COLOR_PAIR(nextTetrominoType + 1));

        wrefresh(score_win);
        wrefresh(level_win);
        wrefresh(lines_win);
        wrefresh(next_display_win);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    endwin();
}

void SingleplayerTetrisGame::init_colors(void) {
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Error - no color support on this terminal\n");
        exit(1);
    }

    start_color();

    // In case the terminal supports custom colors
    if (COLORS >= CUSTOM_ORANGE && COLOR_PAIRS >= CUSTOM_ORANGE && can_change_color()) {
        init_color(CUSTOM_RED, 956, 262, 211);
        init_color(CUSTOM_GREEN, 298, 686, 313);
        init_color(CUSTOM_YELLOW, 1000, 921, 230);
        init_color(CUSTOM_BLUE, 247, 317, 860);
        init_color(CUSTOM_CYAN, 0, 737, 831);
        init_color(CUSTOM_ORANGE, 1000, 596, 0);

        init_pair(Color::CYAN, COLOR_BLACK, CUSTOM_CYAN);
        init_pair(Color::BLUE, COLOR_BLACK, CUSTOM_BLUE);
        init_pair(Color::ORANGE, COLOR_BLACK, CUSTOM_ORANGE);
        init_pair(Color::YELLOW, COLOR_BLACK, CUSTOM_YELLOW);
        init_pair(Color::GREEN, COLOR_BLACK, CUSTOM_GREEN);
        init_pair(Color::PURPLE, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(Color::RED, COLOR_BLACK, CUSTOM_RED);
    } else {
        init_pair(Color::CYAN, COLOR_BLACK, COLOR_CYAN);
        init_pair(Color::BLUE, COLOR_BLACK, COLOR_BLUE);
        init_pair(Color::ORANGE, COLOR_BLACK, COLOR_WHITE);
        init_pair(Color::YELLOW, COLOR_BLACK, COLOR_YELLOW);
        init_pair(Color::GREEN, COLOR_BLACK, COLOR_GREEN);
        init_pair(Color::PURPLE, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(Color::RED, COLOR_BLACK, COLOR_RED);
    }
}

void SingleplayerTetrisGame::updateGame() {
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
        spawnTetromino();
        break;
    case 'x':
        SRSManager::rotateTetromino(currentTetromino, RotationDirection::CLOCKWISE, playfield);
        break;
    case 'z':
        SRSManager::rotateTetromino(currentTetromino, RotationDirection::COUNTERCLOCKWISE, playfield);
        break;
    }
}

void SingleplayerTetrisGame::gameOver() {
    gameIsOver = true;
    std::cout << "Game Over!" << std::endl;
}

unsigned int SingleplayerTetrisGame::getLevel() {
    return totalLinesCleared / 10;
}

void SingleplayerTetrisGame::spawnTetromino() {
    currentTetromino = Tetromino(nextTetrominoType, BASE_SPAWN_POSITION);
    nextTetrominoType = bagRandomGenerator.getNextTetrominoType();

    if (playfield.tetrominoFits(currentTetromino)) return;

    // Tetromino does not fit, try to spawn it one row higher
    currentTetromino.setPosition(BASE_SPAWN_POSITION + Vector2<int>(0, 1));
    if (!playfield.tetrominoFits(currentTetromino)) gameOver();
};

void SingleplayerTetrisGame::tryMove(Vector2<int> direction) {
    currentTetromino.setPosition(currentTetromino.getPosition() + direction);
    if (!playfield.tetrominoFits(currentTetromino)) {
        currentTetromino.setPosition(currentTetromino.getPosition() - direction);
    }
}

int SingleplayerTetrisGame::calculateScore(int linesCleared) {
    int level = getLevel();

    switch (linesCleared) {
    case 0: return 0;
    case 1: return 40 * (level + 1);
    case 2: return 100 * (level + 1);
    case 3: return 300 * (level + 1);
    case 4: return 1200 * (level + 1);
    }
}

void SingleplayerTetrisGame::lockTetromino() {
    playfield.lockTetromino(currentTetromino);

    int linesCleared = playfield.attemptClearLines();
    totalLinesCleared += linesCleared;
    score += calculateScore(linesCleared);
}

void SingleplayerTetrisGame::tickGravity() {
    currentTetromino.setPosition(currentTetromino.getPosition() + DOWN_VECTOR);

    if (!playfield.tetrominoFits(currentTetromino)) {
        currentTetromino.setPosition(currentTetromino.getPosition() - DOWN_VECTOR);
        lockTetromino();
        spawnTetromino();
    }

    resetTicksTillGravity();
}

void SingleplayerTetrisGame::resetTicksTillGravity() {
    ticksTillGravity = GRAVITY[std::min(getLevel(), (unsigned int)MAX_DEFINED_LEVEL)];
}
#include "tetris/tetris_game_renderer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

#define CUSTOM_RED 16
#define CUSTOM_GREEN 17
#define CUSTOM_YELLOW 18
#define CUSTOM_BLUE 19
#define CUSTOM_CYAN 20
#define CUSTOM_ORANGE 21

#define WIN_HEIGHT 46
#define WIN_WIDTH 102

TetrisGameRenderer::TetrisGameRenderer() {
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

    initColors();

    outerWin = newwin(WIN_HEIGHT, WIN_WIDTH, 0, 0);

    refresh();

    box(outerWin, 0, 0);
    wrefresh(outerWin);
}

TetrisGameRenderer::~TetrisGameRenderer() {
    endwin();
}

void TetrisGameRenderer::initColors(void) {
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
};
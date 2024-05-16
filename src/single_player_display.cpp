#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

#include "tetris/tetromino.hpp"
#include "display.hpp"
#include <iostream>
using namespace std;

#define WIN_HEIGHT 46
#define WIN_WIDTH 102
#define START_X 0
#define START_Y 0
#define MATRIX_COLS 10
#define MATRIX_ROWS 20

int single_player_display() {
    // WINDOWS
    WINDOW *outer_win;
    WINDOW *display_win;
    WINDOW *title_win;
    WINDOW *score_win;
    WINDOW *level_win;
    WINDOW *lines_win;
    WINDOW *next_win;
    WINDOW *next_display_win;

    // Test Matrix (To be removed in a future)
    int matrix[MATRIX_ROWS][MATRIX_COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 5, 5, 3, 3, 0, 0, 0},
        {0, 1, 5, 5, 0, 0, 3, 5, 0, 0},
        {7, 7, 6, 6, 6, 2, 3, 5, 5, 0},
        {0, 7, 7, 6, 0, 2, 4, 4, 5, 0},
        {1, 1, 1, 1, 2, 2, 4, 4, 0, 0}};

    // Screen initialization
    initscr();
    if (LINES < WIN_HEIGHT || COLS < WIN_WIDTH) {
        endwin();
        cout << "Terminal window too small!" << endl;
        return 1;
    }

    cbreak();             // Don't wait for Enter key
    noecho();             // Don't echo any keypresses
    keypad(stdscr, TRUE); // Allow for arrow key input
    curs_set(0);          // Hide cursor

    init_colors();

    outer_win = newwin(WIN_HEIGHT, WIN_WIDTH, START_X, START_Y);
    display_win = newwin(42, 42, 2, 4);
    title_win = newwin(10, 50, 4, 50);

    // Data windows: Score, Level, Lines
    score_win = newwin(3, 46, 16, 51);
    level_win = newwin(3, 46, 20, 51);
    lines_win = newwin(3, 46, 24, 51);

    next_win = newwin(13, 46, 29, 51);
    next_display_win = newwin(8, 16, 32, 67);

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

    mvwprintw(score_win, 1, 5, "SCORE: \t\t\t 100000");
    mvwprintw(level_win, 1, 5, "LEVEL: \t\t\t 30");
    mvwprintw(lines_win, 1, 5, "LINES: \t\t\t 50");

    mvwprintw(next_win, 0, 0, " ////////////////////////////////////////////  \n"
                              "    \t\tNEXT TETROMINO:                           \n");

    mvwprintw(next_win, 12, 0, " \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");

    box(outer_win, 0, 0);
    box(display_win, 0, 0);
    box(score_win, 0, 0);
    box(level_win, 0, 0);
    box(lines_win, 0, 0);
    box(next_display_win, 0, 0);

    wrefresh(outer_win);
    wrefresh(display_win);
    wrefresh(title_win);
    wrefresh(score_win);
    wrefresh(level_win);
    wrefresh(lines_win);
    wrefresh(next_win);
    wrefresh(next_display_win);

    paint_matrix(display_win, matrix);

    getch();

    endwin();
    return 0;
}
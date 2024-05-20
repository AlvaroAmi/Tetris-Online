#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

#include "display.hpp"
#include "tetris/tetromino.hpp"
#include <iostream>
using namespace std;

#define WIN_HEIGHT 46
#define WIN_WIDTH 102
#define START_X 0
#define START_Y 0
#define MATRIX_COLS 10
#define MATRIX_ROWS 20

int main() {
    // WINDOWS
    WINDOW *outer_win;
    WINDOW *title_win;
    WINDOW *you_lost;
    WINDOW *score_win;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();

    outer_win = newwin(WIN_HEIGHT, WIN_WIDTH, START_X, START_Y);
    title_win = newwin(12, 55, 5, 25);
    you_lost = newwin(11, 80, 18, 10);
    score_win = newwin(3, 50, 35, 25);
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

    mvwprintw(you_lost, 0, 0, "      *******************************************************************\n"
                              "      * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"
                              "      * *    __     ______  _    _   _      ____   _____ _______      * *\n"
                              "      * *    \\ \\   / / __ \\| |  | | | |    / __ \\ / ____|__   __|     * *\n"
                              "      * *     \\ \\_/ / |  | | |  | | | |   | |  | | (___    | |        * *\n"
                              "      * *      \\   /| |  | | |  | | | |   | |  | |\\___ \\   | |        * *\n"
                              "      * *       | | | |__| | |__| | | |___| |__| |____) |  | |        * *\n"
                              "      * *       |_|  \\____/ \\____/  |______\\____/|_____/   |_|        * *\n"
                              "      * *                                                             * *\n"
                              "      * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"
                              "      *******************************************************************\n");

    wattron(score_win, A_BOLD);

    mvwprintw(score_win, 1, 5, "YOUR SCORE: \t\t\t 100000");

    box(outer_win, 0, 0);
    box(score_win, 0, 0);
    wrefresh(outer_win);
    wrefresh(title_win);
    wrefresh(you_lost);
    wrefresh(score_win);

    getch();

    endwin();
    return 0;
}
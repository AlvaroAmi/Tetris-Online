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

#define WIN_HEIGHT 50
#define WIN_WIDTH 180
#define START_X 0
#define START_Y 0
#define MATRIX_COLS 10
#define MATRIX_ROWS 20

int main(/*const char* player1_username, const char* player2_username*/) {
    // WINDOWS
    WINDOW *outer_win;
    WINDOW *title_win;
    WINDOW *player1_username_win;
    WINDOW *player2_username_win;
    WINDOW *player1_display_win;
    WINDOW *player2_display_win;
    WINDOW *player1_score_win;
    WINDOW *player2_score_win;
    WINDOW *player1_next_win;
    WINDOW *player2_next_win;
    WINDOW *player1_next_display_win;
    WINDOW *player2_next_display_win;

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
    player1_display_win = newwin(42, 42, 6, 4);
    player2_display_win = newwin(42, 42, 6, 134);
    title_win = newwin(10, 60, 7, 60);

    player1_username_win = newwin(3, 42, 2, 4);
    player2_username_win = newwin(3, 42, 2, 134);

    // Data windows: Score Counter for each player
    player1_score_win = newwin(3, 35, 26, 51);
    player2_score_win = newwin(3, 35, 26, 93);

    player1_next_win = newwin(13, 46, 34, 51);
    player2_next_win = newwin(13, 40, 34, 93);

    player1_next_display_win = newwin(8, 16, 36, 61);
    player2_next_display_win = newwin(8, 16, 36, 103);

    refresh();

    mvwprintw(title_win, 0, 0, "  //////////////////////////////////////////////////////  \n"
                               "  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  \n"
                               "             _____ _____ _____ ____  ___ ____        \n"
                               "            |_   _| ____|_   _|  _ \\|_ _/ ___|       \n"
                               "              | | |  _|   | | | |_) || |\\___ \\      \n"
                               "              | | | |___  | | |  _ < | | ___)|        \n"
                               "              |_| |_____| |_| |_| \\_\\___|____/      \n"
                               "                                                      \n"
                               "  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  \n"
                               "  //////////////////////////////////////////////////////  \n");

    wattron(player1_score_win, A_BOLD);
    wattron(player2_score_win, A_BOLD);

    mvwprintw(player1_username_win, 1, 1, "\t      PLAYER 1 USERNAME");
    mvwprintw(player2_username_win, 1, 1, "\t      PLAYER 2 USERNAME");

    mvwprintw(player1_score_win, 1, 5, "SCORE: \t         100000");
    mvwprintw(player2_score_win, 1, 5, "SCORE: \t         100000");

    mvwprintw(player1_next_win, 0, 0, " //////////////////////////////////  \n"
                              "           NEXT TETROMINO:                           \n");
    mvwprintw(player1_next_win, 12, 0, " \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");

    mvwprintw(player2_next_win, 0, 0, " //////////////////////////////////  \n"
                              "           NEXT TETROMINO:                           \n");
    mvwprintw(player2_next_win, 12, 0, " \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");

    box(outer_win, 0, 0);
    box(player1_display_win, 0, 0);
    box(player1_username_win, 0, 0);
    box(player1_score_win, 0, 0);
    box(player1_next_display_win, 0, 0);

    box(player2_display_win, 0, 0);
    box(player2_username_win, 0, 0);
    box(player2_score_win, 0, 0);
    box(player2_next_display_win, 0, 0);
    
    wborder(player1_username_win, ' ', ' ', '*', '*', '*', '*', '*', '*');
    wborder(player2_username_win, ' ', ' ', '*', '*', '*', '*', '*', '*');

    wrefresh(outer_win);
    wrefresh(title_win);

    wrefresh(player1_username_win);
    wrefresh(player1_display_win);
    wrefresh(player1_score_win);
    wrefresh(player1_next_win);
    wrefresh(player1_next_display_win);

    wrefresh(player2_username_win);
    wrefresh(player2_display_win);
    wrefresh(player2_score_win);
    wrefresh(player2_next_win);
    wrefresh(player2_next_display_win);
    
    getch();

    endwin();
    return 0;
}
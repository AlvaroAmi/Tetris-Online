#include <ncurses.h>
#include <iostream>
using namespace std;

#define WIN_HEIGHT 44
#define WIN_WIDTH 100
#define START_X 0
#define START_Y 0

int show_display() {
    // WINDOWS
    WINDOW *outer_win;
    WINDOW *display_win;
    WINDOW *title_win;
    WINDOW *score_win;
    WINDOW *level_win;
    WINDOW *lines_win;
    WINDOW *next_win;
    WINDOW *next_display_win;

    //Screen initialization
    initscr();
    if (LINES < WIN_HEIGHT || COLS < WIN_WIDTH) {
        endwin();
        cout << "Terminal window too small!" << endl;
        return 1;
    }

    outer_win = newwin(WIN_HEIGHT, WIN_WIDTH, START_X, START_Y);
    display_win = newwin(40, 40, 2, 4);
    title_win = newwin(10, 50, 4, 48);

    // Data windows: Score, Level, Lines
    score_win = newwin(3, 46, 16, 49);
    level_win = newwin(3, 46, 20, 49);
    lines_win = newwin(3, 46, 24, 49);

    next_win = newwin(13, 46, 29, 49);
    next_display_win = newwin(8, 16, 32, 65);
    
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

    getch(); 

    endwin();
    return 0;
}
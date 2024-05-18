#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

#include "tetris/tetromino.hpp"
#include <iostream>
#include "display.hpp"
using namespace std;

#define WIN_HEIGHT 46
#define WIN_WIDTH 102
#define START_X 0
#define START_Y 0
#define MATRIX_COLS 10
#define MATRIX_ROWS 20

#define CUSTOM_RED 16
#define CUSTOM_GREEN 17
#define CUSTOM_YELLOW 18
#define CUSTOM_BLUE 19
#define CUSTOM_CYAN 20
#define CUSTOM_ORANGE 21

void init_colors(void);
int paint_matrix(WINDOW *win, int (*matrix)[10]);
int paint_next_tetromino(WINDOW *win, TetrominoType tt);

int single_player() {
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

void init_colors(void) {
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

        init_pair(Color::CYAN, CUSTOM_CYAN, COLOR_BLACK);
        init_pair(Color::BLUE, CUSTOM_BLUE, COLOR_BLACK);
        init_pair(Color::ORANGE, CUSTOM_ORANGE, COLOR_BLACK);
        init_pair(Color::YELLOW, CUSTOM_YELLOW, COLOR_BLACK);
        init_pair(Color::GREEN, CUSTOM_GREEN, COLOR_BLACK);
        init_pair(Color::PURPLE, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(Color::RED, CUSTOM_RED, COLOR_BLACK);
    } else {
        init_pair(Color::CYAN, COLOR_CYAN, COLOR_BLACK);
        init_pair(Color::BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(Color::ORANGE, COLOR_WHITE, COLOR_BLACK);
        init_pair(Color::YELLOW, COLOR_YELLOW, COLOR_BLACK);
        init_pair(Color::GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(Color::PURPLE, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(Color::RED, COLOR_RED, COLOR_BLACK);
    }
}

int paint_matrix(WINDOW *win, int (*matrix)[10]) {
    start_color();
    wattron(win, A_REVERSE);

    int row;
    int column;
    for (row = 0; row < MATRIX_ROWS; row++) {
        for (column = 0; column < MATRIX_COLS; column++) {
            int value = matrix[row][column];
            int x_coordinate = 1 + column * 4;
            int y_coordinate = 1 + row * 2;

            if (value == 0) {
                wattron(win, COLOR_PAIR(9));
                mvwaddstr(win, y_coordinate, x_coordinate, "    ");
                mvwaddstr(win, y_coordinate + 1, x_coordinate, "    ");
                wattroff(win, COLOR_PAIR(9));
            } else {
                wattron(win, COLOR_PAIR(value));
                mvwaddstr(win, y_coordinate, x_coordinate, "    ");
                mvwaddstr(win, y_coordinate + 1, x_coordinate, "    ");
                wattroff(win, COLOR_PAIR(value));
            }

            wrefresh(win);
        }
    }
    wattroff(win, A_REVERSE);
    return 0;
}

int paint_next_tetromino(WINDOW *win, TetrominoType tt) {
    const int (*matrix)[5];
    const int I[5][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {1,1,1,1,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    const int J[5][5] = {{0,0,0,0,0}, {0,0,0,2,0}, {0,2,2,2,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    const int L[5][5] = {{0,0,0,0,0}, {0,3,0,0,0}, {0,3,3,3,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    const int O[5][5] = {{0,4,4,0,0}, {0,4,4,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    const int S[5][5] = {{0,0,5,5,0}, {0,5,5,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    const int Z[5][5] = {{0,7,7,0,0}, {0,0,7,7,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    const int T[5][5] = {{0,6,6,6,0}, {0,0,6,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};

    int x_centering_coefficient, y_centering_coefficient = 0;
           
    start_color();
    init_colors();
    wattron(win, A_REVERSE);
        
    switch (tt) {
        case TetrominoType::I: 
            matrix = I; 
            x_centering_coefficient = 3;
            break;
        case TetrominoType::J: 
            matrix = J; 
            x_centering_coefficient = 1; 
            y_centering_coefficient = 1;
            break;
        case TetrominoType::L:
            matrix = L;
            x_centering_coefficient = 1; 
            y_centering_coefficient = 1;
            break;
        case TetrominoType::O:
            matrix = O;
            x_centering_coefficient = 3; 
            y_centering_coefficient = 3;
            break;
        case TetrominoType::S:
            matrix = S;
            x_centering_coefficient = 1;
            y_centering_coefficient = 3;
            break;
        case TetrominoType::Z:
            matrix = Z;
            x_centering_coefficient = 1;
            y_centering_coefficient = 3;
            break;
        case TetrominoType::T:
            matrix = T;
            x_centering_coefficient = 1;
            y_centering_coefficient = 3;
            break;
        default:
            return 1;
    }

    int row, column;
    for (row = 0; row < 5; row++) {
        for (column = 0; column < 5; column++) {
            int value = matrix[row][column];
            int x_coordinate = x_centering_coefficient + 1 + column * 4;
            int y_coordinate = y_centering_coefficient + 1 + row * 2;

            if (value == 0) {value = 9;}
            wattron(win, COLOR_PAIR(value));
            mvwaddstr(win, y_coordinate, x_coordinate, "    ");
            mvwaddstr(win, y_coordinate + 1, x_coordinate, "    ");
            wattroff(win, COLOR_PAIR(value));

            wrefresh(win);
        }
    }
    wattroff(win, A_REVERSE);
    box(win, 0, 0);
    wrefresh(win);
    
    return 0;
}
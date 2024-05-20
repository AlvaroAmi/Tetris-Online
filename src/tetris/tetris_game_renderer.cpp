#include "tetris/tetris_game_renderer.hpp"

#define CUSTOM_RED 16
#define CUSTOM_GREEN 17
#define CUSTOM_YELLOW 18
#define CUSTOM_BLUE 19
#define CUSTOM_CYAN 20
#define CUSTOM_ORANGE 21
#define XTERM_GRAY 8

TetrisGameRenderer::TetrisGameRenderer(int width, int height) {
    initscr();
    if (LINES < height || COLS < width) {
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

    outerWin = newwin(height, width, 0, 0);

    refresh();

    box(outerWin, 0, 0);
    wrefresh(outerWin);
}

TetrisGameRenderer::~TetrisGameRenderer() {
    delwin(outerWin);
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

    if (COLORS >= 8 && COLOR_PAIRS >= 8) {
        init_pair(Color::GRAY, COLOR_BLACK, XTERM_GRAY);
    }
    // We will fallback to white in the renderer. Not ideal but we don't have any colors left
};

void TetrisGameRenderer::displayNextTetromino(WINDOW *win, TetrominoType tetrominoType) const {
    const int(*matrix)[5];
    const int I[5][5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
    const int J[5][5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 2, 0}, {0, 2, 2, 2, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
    const int L[5][5] = {{0, 0, 0, 0, 0}, {0, 3, 0, 0, 0}, {0, 3, 3, 3, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
    const int O[5][5] = {{0, 4, 4, 0, 0}, {0, 4, 4, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
    const int S[5][5] = {{0, 0, 5, 5, 0}, {0, 5, 5, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
    const int Z[5][5] = {{0, 7, 7, 0, 0}, {0, 0, 7, 7, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
    const int T[5][5] = {{0, 6, 6, 6, 0}, {0, 0, 6, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};

    int x_centering_coefficient, y_centering_coefficient = 0;

    switch (tetrominoType) {
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
    }

    int row, column;
    for (row = 0; row < 5; row++) {
        for (column = 0; column < 5; column++) {
            int value = matrix[row][column];
            int x_coordinate = x_centering_coefficient + 1 + column * 4;
            int y_coordinate = y_centering_coefficient + 1 + row * 2;

            if (value == 0) {
                value = 9;
            }
            wattron(win, COLOR_PAIR(value));
            mvwaddstr(win, y_coordinate, x_coordinate, "    ");
            mvwaddstr(win, y_coordinate + 1, x_coordinate, "    ");
            wattroff(win, COLOR_PAIR(value));
        }
    }
    box(win, 0, 0);
    wnoutrefresh(win);
}
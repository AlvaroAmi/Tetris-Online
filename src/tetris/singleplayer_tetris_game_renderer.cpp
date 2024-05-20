#include "tetris/singleplayer_tetris_game_renderer.hpp"

#define WIN_HEIGHT 46
#define WIN_WIDTH 102

SingleplayerTetrisGameRenderer::SingleplayerTetrisGameRenderer(const SingleplayerTetrisGame &game)
    : TetrisGameRenderer(WIN_WIDTH, WIN_HEIGHT), game(game) {
    titleWin = newwin(10, 50, 4, 50);

    scoreWin = newwin(3, 46, 16, 51);
    levelWin = newwin(3, 46, 20, 51);
    linesWin = newwin(3, 46, 24, 51);

    nextWin = newwin(15, 46, 28, 55);
    nextDisplayWin = newwin(12, 24, 30, 61);

    refresh();

    mvwprintw(titleWin, 0, 0, "  ////////////////////////////////////////////  \n"
                              "  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  \n"
                              "        _____ _____ _____ ____  ___ ____        \n"
                              "       |_   _| ____|_   _|  _ \\|_ _/ ___|       \n"
                              "         | | |  _|   | | | |_) || |\\___ \\      \n"
                              "         | | | |___  | | |  _ < | | ___)|        \n"
                              "         |_| |_____| |_| |_| \\_\\___|____/      \n"
                              "                                                 \n"
                              "  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  \n"
                              "  ////////////////////////////////////////////  \n");

    wattron(scoreWin, A_BOLD);
    wattron(levelWin, A_BOLD);
    wattron(linesWin, A_BOLD);

    mvwprintw(scoreWin, 1, 5, "SCORE: \t\t\t 0");
    mvwprintw(levelWin, 1, 5, "LEVEL: \t\t\t 0");
    mvwprintw(linesWin, 1, 5, "LINES: \t\t\t 0");

    mvwprintw(nextWin, 0, 0, " //////////////////////////////////  \n"
                             "       NEXT TETROMINO:                           \n");
    mvwprintw(nextWin, 14, 0, " \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");

    box(scoreWin, 0, 0);
    box(levelWin, 0, 0);
    box(linesWin, 0, 0);
    box(nextDisplayWin, 0, 0);

    wrefresh(titleWin);
    wrefresh(scoreWin);
    wrefresh(levelWin);
    wrefresh(linesWin);
    wrefresh(nextWin);
    wrefresh(nextDisplayWin);

    playfieldRenderer = PlayfieldRenderer(4, 2, game.playfield);
}

SingleplayerTetrisGameRenderer::~SingleplayerTetrisGameRenderer() {
    delwin(titleWin);
    delwin(scoreWin);
    delwin(levelWin);
    delwin(linesWin);
    delwin(nextWin);
    delwin(nextDisplayWin);
}

void SingleplayerTetrisGameRenderer::render() const {
    playfieldRenderer.renderPlayfield(game.currentTetromino);

    mvwprintw(scoreWin, 1, 5, "SCORE: \t\t\t %d", game.score);
    mvwprintw(levelWin, 1, 5, "LEVEL: \t\t\t %d", game.getLevel());
    mvwprintw(linesWin, 1, 5, "LINES: \t\t\t %d", game.totalLinesCleared);

    displayNextTetromino(nextDisplayWin, game.nextTetrominoType);

    wnoutrefresh(scoreWin);
    wnoutrefresh(levelWin);
    wnoutrefresh(linesWin);
    wnoutrefresh(nextDisplayWin);
    doupdate();
}
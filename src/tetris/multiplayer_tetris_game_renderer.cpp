#include "tetris/multiplayer_tetris_game_renderer.hpp"

#define WIN_HEIGHT 50
#define WIN_WIDTH 180

MultiplayerTetrisGameRenderer::MultiplayerTetrisGameRenderer(const MultiplayerTetrisGame &game)
    : TetrisGameRenderer(WIN_WIDTH, WIN_HEIGHT), game(game) {
    playfieldRenderer = PlayfieldRenderer(4, 6, game.playfield);
    enemyPlayfieldRenderer = PlayfieldRenderer(134, 6, game.enemyPlayfield);
    title_win = newwin(10, 60, 7, 60);

    player1_username_win = newwin(3, 42, 2, 4);
    player2_username_win = newwin(3, 42, 2, 134);

    // Data windows: Score Counter for each player
    player1_garbage_win = newwin(3, 35, 26, 51);
    player2_garbage_win = newwin(3, 35, 26, 93);

    player1_next_win = newwin(15, 46, 32, 51);
    player2_next_win = newwin(15, 40, 32, 93);

    player1_next_display_win = newwin(12, 24, 34, 57);
    player2_next_display_win = newwin(12, 24, 34, 99);

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

    wattron(player1_garbage_win, A_BOLD);
    wattron(player2_garbage_win, A_BOLD);

    mvwprintw(player1_username_win, 1, 1, "\t      PLAYER 1 USERNAME");
    mvwprintw(player2_username_win, 1, 1, "\t      PLAYER 2 USERNAME");

    mvwprintw(player1_garbage_win, 1, 5, "GARBAGE SENT: \t     0");
    mvwprintw(player2_garbage_win, 1, 5, "GARBAGE SENT: \t     0");

    mvwprintw(player1_next_win, 0, 0, " //////////////////////////////////  \n"
                                      "       NEXT TETROMINO:                           \n");
    mvwprintw(player1_next_win, 14, 0, " \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");

    mvwprintw(player2_next_win, 0, 0, " //////////////////////////////////  \n"
                                      "       NEXT TETROMINO:                           \n");
    mvwprintw(player2_next_win, 14, 0, " \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");

    box(player1_username_win, 0, 0);
    box(player1_garbage_win, 0, 0);
    box(player1_next_display_win, 0, 0);

    box(player2_username_win, 0, 0);
    box(player2_garbage_win, 0, 0);
    box(player2_next_display_win, 0, 0);

    wborder(player1_username_win, ' ', ' ', '*', '*', '*', '*', '*', '*');
    wborder(player2_username_win, ' ', ' ', '*', '*', '*', '*', '*', '*');

    wrefresh(title_win);

    wrefresh(player1_username_win);
    wrefresh(player1_garbage_win);
    wrefresh(player1_next_win);
    wrefresh(player1_next_display_win);

    wrefresh(player2_username_win);
    wrefresh(player2_garbage_win);
    wrefresh(player2_next_win);
    wrefresh(player2_next_display_win);
}

MultiplayerTetrisGameRenderer::~MultiplayerTetrisGameRenderer() {
    delwin(title_win);
    delwin(player1_username_win);
    delwin(player2_username_win);
    delwin(player1_garbage_win);
    delwin(player2_garbage_win);
    delwin(player1_next_win);
    delwin(player2_next_win);
    delwin(player1_next_display_win);
    delwin(player2_next_display_win);
}

void MultiplayerTetrisGameRenderer::render() const {
    playfieldRenderer.renderPlayfield(game.currentTetromino);
    // enemyPlayfieldRenderer.renderPlayfield();

    mvwprintw(player1_garbage_win, 1, 5, "GARBAGE SENT: \t     %d", game.garbageLinesSent);
    mvwprintw(player2_garbage_win, 1, 5, "GARBAGE SENT: \t     %d", game.garbageLinesReceived);

    displayNextTetromino(player1_next_display_win, game.nextTetrominoType);

    wrefresh(player1_garbage_win);
    wrefresh(player1_next_display_win);
}

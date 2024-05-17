#include "tetris/enemy_player_display.hpp"

/* EnemyPlayerDisplay::EnemyPlayerDisplay() {
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    refresh();
    enemy_win = newwin(20, 20, 0, 0);
    box(enemy_win, 0, 0);
    wrefresh(enemy_win);
}

EnemyPlayerDisplay::~EnemyPlayerDisplay() {
    delwin(enemy_win);
} */

void EnemyPlayerDisplay::updateGame(std::string &matrixString) {
    for (int y = 0; y < VISIBLE_PLAYFIELD_HEIGHT; y++) {
        for (int x = 0; x < PLAYFIELD_WIDTH; x++) {
            Color cellColor = (Color)(matrixString.at(y * PLAYFIELD_WIDTH + x) - '0');
            playfield.setTile(x, y, cellColor);
        }
    }
}
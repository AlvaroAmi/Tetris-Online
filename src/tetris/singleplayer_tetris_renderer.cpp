#include "tetris/singleplayer_tetris_game_renderer.hpp"

SingleplayerTetrisGameRenderer::SingleplayerTetrisGameRenderer() {
    outerWin = newwin(WIN_HEIGHT, WIN_WIDTH, 0, 0);
    initColors();
}

SingleplayerTetrisGameRenderer::~SingleplayerTetrisGameRenderer() {
    delwin(outerWin);
}
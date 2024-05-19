#include "tetris/tetris_c_api.h"
#include "tetris/singleplayer_tetris_game.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void *create_singleplayer_tetris_game() {
    return new SingleplayerTetrisGame();
}

#ifdef __cplusplus
}
#endif
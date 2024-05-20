#include "tetris/tetris_c_api.h"
#include "tetris/multiplayer_tetris_game.hpp"
#include "tetris/singleplayer_tetris_game.hpp"
#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif

void *create_singleplayer_tetris_game(SOCKET sock) {
    return new SingleplayerTetrisGame(sock);
}

void *create_multiplayer_tetris_game(SOCKET sock) {
    return new MultiplayerTetrisGame(sock);
}

void destroy_multiplayer_game(multiplayer_tetris_game game) {
    delete game;
}

void update_enemy_playfield(singleplayer_tetris_game game, const char *matrix_c_string) {
    std::string matrixString(matrix_c_string);
    ((MultiplayerTetrisGame *)game)->updateEnemyPlayfield(matrixString);
}

void enqueue_garbage(multiplayer_tetris_game game, int lines) {
    ((MultiplayerTetrisGame *)game)->enqueueGarbage(lines);
}

#ifdef __cplusplus
}
#endif
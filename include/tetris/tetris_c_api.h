#ifndef TETRIS_C_API_H
#define TETRIS_C_API_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

typedef void *singleplayer_tetris_game;
typedef void *multiplayer_tetris_game;

EXTERNC singleplayer_tetris_game create_singleplayer_tetris_game(SOCKET sock);

EXTERNC multiplayer_tetris_game create_multiplayer_tetris_game(SOCKET sock);
EXTERNC void destroy_multiplayer_game(multiplayer_tetris_game game);
EXTERNC void update_enemy_playfield(singleplayer_tetris_game game, char *matrix_c_string);
EXTERNC void enqueue_garbage(multiplayer_tetris_game game, int lines);

#undef EXTERNC

#endif // TETRIS_C_API_H
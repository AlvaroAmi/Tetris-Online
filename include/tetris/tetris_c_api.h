#ifndef TETRIS_C_API_H
#define TETRIS_C_API_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

typedef void *singleplayer_tetris_game;
typedef void *multiplayer_tetris_game;

EXTERNC singleplayer_tetris_game create_singleplayer_tetris_game();
/* EXTERNC void destroy_singleplayer_game(SingleplayerTetrisGame game);

EXTERNC MultiplayerTetrisGame create_multiplayer_game();
EXTERNC void destroy_multiplayer_game(MultiplayerTetrisGame game); */

#undef EXTERNC

#endif // TETRIS_C_API_H
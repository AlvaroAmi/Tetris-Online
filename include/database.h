#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int authenticate_user(sqlite3* db, const char* email, const char* password);
int db_register_user(sqlite3* db, const char* email, const char* password, const char* username);
int delete_user(sqlite3* db, const char* email);
char* get_username(sqlite3* db, int user_id);

int insert_multiplayer_game(sqlite3 *db, int idUser1, int idUser2, int firstWins, const char *start, const char *end);
int insert_singleplayer_game(sqlite3 *db, const char *start_datetime, const char *finish_datetime, int score, int linesCleared, int level, int player_id);
char *calculate_game_duration(sqlite3 *db, int id, int is_multiplayer);
#ifdef __cplusplus
}
#endif

#endif // DATABASE_H

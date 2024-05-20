#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <winsock2.h>

#ifdef __cplusplus
#include <cstring>
#include <string>
using std::string;

extern "C" {
#endif

void send_game_start(SOCKET sock, int user_id);
void send_game_finish(SOCKET sock);

#ifdef __cplusplus
}
void send_game_update(SOCKET sock, const string &matrix);
void send_garbage(SOCKET sock, int lines);
void send_singleplayer_game_finish(SOCKET sock, const char *start_datetime, const char *finish_datetime, int score, int linesCleared, int level);
void send_multiplayer_game_finish(SOCKET sock, int id_user1, int id_user2, bool first_win, const char *start_datetime, const char *finish_datetime);
void start_client();
#endif

#endif // CLIENT_HPP

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <winsock2.h>
#include <cstring>
#include <string>

using std::string;

void send_game_start(SOCKET sock, int user_id);
void send_game_finish(SOCKET sock);
void send_game_update(SOCKET sock, const string& matrix);

#endif


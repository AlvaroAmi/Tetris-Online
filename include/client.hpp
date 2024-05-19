#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstring>
#include <string>
#include <winsock2.h>

using std::string;

void send_game_start(SOCKET sock, int user_id);
void send_game_finish(SOCKET sock);
void send_game_update(SOCKET sock, const string &matrix);
void start_client();

#endif

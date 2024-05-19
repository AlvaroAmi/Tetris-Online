#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <winsock2.h>
#include <cstring>
#include <string>

using std::string;

void send_request(SOCKET sock, const char* command, const char* email, const char* password, const char* username);
int receive_response(SOCKET sock);
void clear_screen();
int has_uppercase(const char *str);
int has_special_char(const char *str);
int contains_at_symbol(const char *email);
void start_listener_thread(SOCKET sock);
void stop_listener_thread();
void send_game_start(SOCKET sock, int user_id);
void send_game_finish(SOCKET sock);
void send_game_update(SOCKET sock, const string& matrix);

#endif


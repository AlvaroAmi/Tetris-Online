#ifndef MENUS_H
#define MENUS_H

#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif

void display_menu(SOCKET sock);
void send_request(SOCKET sock, const char* command, const char* email, const char* password, const char* username);
void receive_response(SOCKET sock);

#ifdef __cplusplus
}
#endif

#endif

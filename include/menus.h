#ifndef MENUS_H
#define MENUS_H

#include <winsock2.h>

#ifdef __cplusplus
extern "C" {
#endif

void display_menu(SOCKET sock);
void send_request(SOCKET sock, const char* command, const char* email, const char* password, const char* username);
int receive_response(SOCKET sock);
extern void start_listener_thread(SOCKET sock);
extern void stop_listener_thread();


#ifdef __cplusplus
}
#endif

#endif

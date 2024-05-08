#include <iostream>
#include <winsock2.h>
#include <cstring>
#include <string>
#include "menus.h"

using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

// Function to initialize Winsock and connect to the server
SOCKET connect_to_server() {
    WSADATA wsa_data;
    int wsaStart = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wsaStart != 0) {
        cerr << "Failed to initialize Winsock: " << WSAGetLastError() << endl;
        return INVALID_SOCKET;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Socket creation failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cerr << "Connect failed with error: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return INVALID_SOCKET;
    }

    cout << "Connected to server." << endl;
    return sock;
}

void send_request(SOCKET sock, const char* command, const char* email, const char* password, const char* username) {
    char message[256]; 

    if (username != NULL) {
        sprintf(message, "%s|%s|%s|%s", command, email, password, username);
    } else {
        sprintf(message, "%s|%s|%s", command, email, password);
    }

    if (send(sock, message, strlen(message), 0) < 0) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        return;
    }


}

int receive_response(SOCKET sock) {
    char server_reply[200];
    int recv_size;

    if ((recv_size = recv(sock, server_reply, 200, 0)) == SOCKET_ERROR) {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
        return 0; 
    }

    server_reply[recv_size] = '\0'; 

    cout << "Server reply: " << server_reply << endl;

    if (strcmp(server_reply, "Login successful") == 0 || strcmp(server_reply, "Registration successful") == 0) {
        return 1; 
    }

    return 0; 
}

int main() {
    SOCKET sock = connect_to_server();
    if (sock != INVALID_SOCKET) {
        display_menu(sock);

        closesocket(sock);
        WSACleanup();
    }
    return 0;
}

#include <iostream>
#include <fstream>
#include <ctime>
#include <winsock2.h>
#include <cstring>
#include <string>
#include "menus.h"
#include "config_file_parser.h"

using namespace std;

#define DEFAULT_SERVER_IP "127.0.0.1" //Default values if config file fails
#define DEFAULT_SERVER_PORT 6000

ofstream logFile("client_log.log", ios::app); 

void log(const string& message, const string& level) {
    time_t now = time(0);
    char dt[100];
    strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
    logFile << "[" << dt << "] [" << level << "] " << message << endl;
}

SOCKET connect_to_server() {
    log("Initializing Winsock", "INFO");
    WSADATA wsa_data;
    int wsaStart = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wsaStart != 0) {
        cerr << "Failed to initialize Winsock: " << WSAGetLastError() << endl;
        log("Failed to initialize Winsock: " + to_string(WSAGetLastError()), "ERROR");
        return INVALID_SOCKET;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Socket creation failed with error: " << WSAGetLastError() << endl;
        log("Socket creation failed with error: " + to_string(WSAGetLastError()), "ERROR");
        WSACleanup();
        return INVALID_SOCKET;
    }

    struct config_parameter *config = read_config_file("clientconfig.txt");
    if (!config) {
        log("Failed to read the client configuration file.", "ERROR");
        return INVALID_SOCKET;
    }
    struct config_parameter *ip_param = get_config_parameter(config, "SERVER_IP");
    struct config_parameter *port_param = get_config_parameter(config, "SERVER_PORT");
    string server_ip = ip_param ? ip_param->value.string : DEFAULT_SERVER_IP;
    int server_port = port_param ? port_param->value.integer : DEFAULT_SERVER_PORT;

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);

    log("Connecting to server " + server_ip + ":" + to_string(server_port), "INFO");
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cerr << "Connect failed with error: " << WSAGetLastError() << endl;
        log("Connect failed with error: " + to_string(WSAGetLastError()), "ERROR");
        closesocket(sock);
        WSACleanup();
        return INVALID_SOCKET;
    }

    cout << "Connected to server." << endl;
    log("Connected to server", "INFO");
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
        log("Send failed: " + to_string(WSAGetLastError()), "ERROR");
        return;
    }
}

int receive_response(SOCKET sock) {
    char server_reply[200];
    int recv_size;

    log("Receiving response from server", "INFO");
    if ((recv_size = recv(sock, server_reply, 200, 0)) == SOCKET_ERROR) {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
        log("recv failed: " + to_string(WSAGetLastError()), "ERROR");
        return 0; 
    }

    server_reply[recv_size] = '\0'; 

    cout << "Server reply: " << server_reply << endl;
    log("Server reply: " + string(server_reply), "INFO");

    if (strcmp(server_reply, "Login successful") == 0 || strcmp(server_reply, "Registration successful") == 0) {
        return 1; 
    }

    return 0; 
}

int main() {
    log("Client application started", "INFO");
    SOCKET sock = connect_to_server();
    if (sock != INVALID_SOCKET) {
        display_menu(sock);

        closesocket(sock);
        log("Socket closed", "INFO");
        WSACleanup();
        log("Winsock cleanup complete", "INFO");
    }
    log("Client application ended", "INFO");
    return 0;
}

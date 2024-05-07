#include <iostream>
#include <winsock2.h>
#include <cstring>
#include <string>
#include <thread> 
#include <sqlite3.h>
#include <iostream>
#include "menus.h"
#include "sqlite3.h"



using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

sqlite3* db;

bool open_database(const char* db_path) {
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

#include <iostream>
#include <sqlite3.h>

bool authenticate_user(const char* email, const char* password) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT 1 FROM USER WHERE email = ? AND password = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    rc = sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding email: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding password: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt); 
        return false;
    }

    rc = sqlite3_step(stmt);
    bool result = (rc == SQLITE_ROW);

    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return result;
}


bool register_user(const char* email, const char* password, const char* username) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO USER (email, password, username) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, username, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error inserting new user: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}



void process_request(char* request, SOCKET communication_socket) {
    char* command = strtok(request, "|");
    char* email = strtok(NULL, "|");
    char* password = strtok(NULL, "|");
    char* username = strtok(NULL, "|");  // Extraer el nombre de usuario si está presente

    if (email == nullptr || password == nullptr) {
        const char* response = "Invalid request format";
        send(communication_socket, response, strlen(response), 0);
        return;
    }

    if (strcmp(command, "LOGIN") == 0) {
        if (authenticate_user(email, password)) {
            const char* response = "Login successful";
            send(communication_socket, response, strlen(response), 0);
        } else {
            const char* response = "Login failed";
            send(communication_socket, response, strlen(response), 0);
        }
    } else if (strcmp(command, "REGISTER") == 0) {
        if (username == nullptr) { // Verificar si el nombre de usuario está presente
            const char* response = "Username required";
            send(communication_socket, response, strlen(response), 0);
            return;
        }
        if (register_user(email, password, username)) {
            const char* response = "Registration successful";
            send(communication_socket, response, strlen(response), 0);
        } else {
            const char* response = "Registration failed";
            send(communication_socket, response, strlen(response), 0);
        }
    } else {
        const char* response = "Unknown command";
        send(communication_socket, response, strlen(response), 0);
    }
}

void client_handler(SOCKET communication_socket) {
    char receive_buffer[512] = {0}; 

    while (true) {
        int bytes_received = recv(communication_socket, receive_buffer, sizeof(receive_buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            cout << "recv failed: " << WSAGetLastError() << endl;
            break;
        } else if (bytes_received == 0) {
            cout << "Client disconnected." << endl;
            break;
        }

        receive_buffer[bytes_received] = '\0'; 
        process_request(receive_buffer, communication_socket);
    }

    closesocket(communication_socket);
}

int main() {
    WSADATA wsa_data;
    SOCKET connection_socket, communication_socket;
    struct sockaddr_in server_address, client_address;

    if (!open_database("TetrisOnline.db")) {
        return 1; 
    }

    cout << "Initialising Winsock..." << endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        cout << "Failed. Error Code : " << WSAGetLastError() << endl;
        return -1;
    }

    cout << "Initialised." << endl;

    connection_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_socket == INVALID_SOCKET) {
        cout << "Could not create socket : " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    cout << "Socket created." << endl;

    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(connection_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        cout << "Bind failed with error code: " << WSAGetLastError() << endl;
        closesocket(connection_socket);
        WSACleanup();
        return -1;
    }

    cout << "Bind done." << endl;

    if (listen(connection_socket, 3) == SOCKET_ERROR) {
        cout << "Listen failed with error code: " << WSAGetLastError() << endl;
        closesocket(connection_socket);
        WSACleanup();
        return -1;
    }

    cout << "Waiting for incoming connections..." << endl;
    int client_size = sizeof(struct sockaddr_in);
    while ((communication_socket = accept(connection_socket, (struct sockaddr*)&client_address, &client_size)) != INVALID_SOCKET) {
        cout << "Connection accepted." << endl;

        
        thread client_thread(client_handler, communication_socket);
        client_thread.detach(); 

        
    }

    closesocket(connection_socket);
    WSACleanup();
    return 0;
}
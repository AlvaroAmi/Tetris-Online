#include <iostream>
#include <fstream>
#include <ctime>
#include <winsock2.h>
#include <cstring>
#include <string>
#include <thread>
#include <sqlite3.h>
#include "menus.h"
#include "config_file_parser.h"  

using namespace std;

#define DEFAULT_SERVER_IP "127.0.0.1" //Valores por defecto si hay un error leyendo del fichero de config
#define DEFAULT_SERVER_PORT 6000

sqlite3* db;
ofstream logFile;

void log(const string& message, const string& level) {
    time_t now = time(0);
    char dt[100];
    strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
    logFile << "[" << dt << "] [" << level << "] " << message << endl;
    cout << "[" << dt << "] [" << level << "] " << message << endl;
}

bool open_database(const char* db_path) {
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        log("Error opening database: " + string(sqlite3_errmsg(db)), "ERROR");
        return false;
    }
    log("Database opened successfully.", "INFO");
    return true;
}

bool authenticate_user(const char* email, const char* password) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT 1 FROM USER WHERE email = ? AND password = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        log("Error preparing login statement: " + string(sqlite3_errmsg(db)), "ERROR");
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC) != SQLITE_OK) {
        log("Error binding login parameters.", "ERROR");
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        log("Login successful for: " + string(email), "INFO");
        sqlite3_finalize(stmt);
        return true;
    } else {
        log("Login failed for: " + string(email), "WARNING");
        sqlite3_finalize(stmt);
        return false;
    }
}

bool register_user(const char* email, const char* password, const char* username) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO USER (email, password, username) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        log("Error preparing register statement: " + string(sqlite3_errmsg(db)), "ERROR");
        return false;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log("Error registering new user: " + string(sqlite3_errmsg(db)), "ERROR");
        sqlite3_finalize(stmt);
        return false;
    }

    log("Registration successful for: " + string(email), "INFO");
    sqlite3_finalize(stmt);
    return true;
}

void process_request(char* request, SOCKET communication_socket) {
    char* command = strtok(request, "|");
    char* email = strtok(NULL, "|");
    char* password = strtok(NULL, "|");
    char* username = strtok(NULL, "|");

    if (email == nullptr || password == nullptr) {
        const char* response = "Invalid request format";
        send(communication_socket, response, strlen(response), 0);
        log("Received invalid request.", "ERROR");
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
        if (username == nullptr) {
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
        log("Received unknown command.", "ERROR");
    }
}

void client_handler(SOCKET communication_socket) {
    char receive_buffer[512] = {0};

    while (true) {
        int bytes_received = recv(communication_socket, receive_buffer, sizeof(receive_buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            log("Receive failed: " + to_string(WSAGetLastError()), "ERROR");
            break;
        } else if (bytes_received == 0) {
            log("Client disconnected.", "INFO");
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

    logFile.open("server.log", ios::out | ios::app);
    if (!logFile.is_open()) {
        cerr << "Failed to open log file." << endl;
        return -1;
    }

    struct config_parameter *config = read_config_file("serverconfig.txt");
    if (!config) {
        log("Failed to read the server configuration file.", "ERROR");
        logFile.close();
        return -1;
    }
    struct config_parameter *ip_param = get_config_parameter(config, "SERVER_IP");
    struct config_parameter *port_param = get_config_parameter(config, "SERVER_PORT");
    string server_ip = ip_param ? ip_param->value.string : DEFAULT_SERVER_IP;
    int server_port = port_param ? port_param->value.integer : DEFAULT_SERVER_PORT;

    if (!open_database("TetrisOnline.db")) {
        logFile.close();
        return 1;
    }

    log("Initialising Winsock...", "INFO");
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        log("WSAStartup failed. Error Code : " + to_string(WSAGetLastError()), "ERROR");
        logFile.close();
        free_config_parameters(config);
        return -1;
    }

    connection_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_socket == INVALID_SOCKET) {
        log("Could not create socket : " + to_string(WSAGetLastError()), "ERROR");
        WSACleanup();
        logFile.close();
        free_config_parameters(config);
        return -1;
    }

    server_address.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);

    if (bind(connection_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        log("Bind failed with error code: " + to_string(WSAGetLastError()), "ERROR");
        closesocket(connection_socket);
        WSACleanup();
        logFile.close();
        free_config_parameters(config);
        return -1;
    }

    if (listen(connection_socket, 3) == SOCKET_ERROR) {
        log("Listen failed with error code: " + to_string(WSAGetLastError()), "ERROR");
        closesocket(connection_socket);
        WSACleanup();
        logFile.close();
        free_config_parameters(config);
        return -1;
    }

    log("Server started. Waiting for incoming connections...", "INFO");
    int client_size = sizeof(struct sockaddr_in);
    while ((communication_socket = accept(connection_socket, (struct sockaddr*)&client_address, &client_size)) != INVALID_SOCKET) {
        log("Connection accepted.", "INFO");

        thread client_thread(client_handler, communication_socket);
        client_thread.detach();
    }

    closesocket(connection_socket);
    WSACleanup();
    logFile.close();
    free_config_parameters(config);
    return 0;
}

#include <iostream>
#include <fstream>
#include <ctime>
#include <winsock2.h>
#include <cstring>
#include <string>
#include <thread>
#include <map>
#include <vector>
#include <mutex>
#include <algorithm>
#include <sqlite3.h>
#include "menus.h"
#include "config_file_parser.h"
#include "database.h"

using namespace std;

#define DEFAULT_SERVER_IP "127.0.0.1"
#define DEFAULT_SERVER_PORT 6000

sqlite3* db;
ofstream logFile;

mutex clients_mutex;
map<int, SOCKET> clients; 
vector<int> matchmaking_queue; 
vector<pair<int, int>> pairs; 
map<int, int> user_to_socket; 
map<int, int> socket_to_user;

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

void add_to_matchmaking_queue(int user_id) {
    lock_guard<mutex> guard(clients_mutex);

    if (find(matchmaking_queue.begin(), matchmaking_queue.end(), user_id) != matchmaking_queue.end()) {
        log("Client " + to_string(user_id) + " is already in the matchmaking queue", "INFO");
        return;
    }

    matchmaking_queue.push_back(user_id);
    log("Client " + to_string(user_id) + " added to matchmaking queue", "INFO");

    if (matchmaking_queue.size() >= 2) {
        int user1 = matchmaking_queue.front();
        matchmaking_queue.erase(matchmaking_queue.begin());
        int user2 = matchmaking_queue.front();
        matchmaking_queue.erase(matchmaking_queue.begin());

        pairs.push_back({user1, user2});
        log("Matched clients " + to_string(user1) + " and " + to_string(user2), "INFO");

        // Map users to their sockets
        SOCKET socket1 = user_to_socket[user1];
        SOCKET socket2 = user_to_socket[user2];

        string message = "MATCHED|" + to_string(user2);
        send(socket1, message.c_str(), message.length(), 0);
        message = "MATCHED|" + to_string(user1);
        send(socket2, message.c_str(), message.length(), 0);
    }
}

void remove_from_matchmaking_queue(int user_id) {
    lock_guard<mutex> guard(clients_mutex);
    matchmaking_queue.erase(remove(matchmaking_queue.begin(), matchmaking_queue.end(), user_id), matchmaking_queue.end());
    log("Client " + to_string(user_id) + " removed from matchmaking queue", "INFO");
}

void forward_to_enemy(int user_id, const string& message) {
    lock_guard<mutex> guard(clients_mutex);

    auto it = find_if(pairs.begin(), pairs.end(), [user_id](const pair<int, int>& p) {
        return p.first == user_id || p.second == user_id;
    });

    if (it != pairs.end()) {
        int enemy_id = (it->first == user_id) ? it->second : it->first;

        if (user_to_socket.find(enemy_id) != user_to_socket.end()) {
            SOCKET enemy_socket = user_to_socket[enemy_id];
            if (send(enemy_socket, message.c_str(), message.length(), 0) < 0) {
                log("Send to enemy failed: " + to_string(WSAGetLastError()), "ERROR");
            } else {
                log("Message sent to enemy (user_id: " + to_string(enemy_id) + ", socket: " + to_string(enemy_socket) + "): " + message, "INFO");
            }
        } else {
            log("Enemy socket not found for user_id: " + to_string(enemy_id), "ERROR");
        }
    } else {
        log("Enemy not found for user_id: " + to_string(user_id), "ERROR");
    }
}


void update_user_socket_mapping(int client_id, int user_id) {
    lock_guard<mutex> guard(clients_mutex);

    if (socket_to_user.find(client_id) != socket_to_user.end()) {
        int old_user_id = socket_to_user[client_id];
        user_to_socket.erase(old_user_id);
    }

    socket_to_user[client_id] = user_id;
    user_to_socket[user_id] = client_id;

    log("Updated user-socket mapping: User " + to_string(user_id) + " -> Socket " + to_string(client_id), "INFO");
}

void process_request(char* request, SOCKET communication_socket, int client_id) {
    char* command = strtok(request, "|");
    char* param1 = strtok(NULL, "|");
    char* param2 = strtok(NULL, "|");
    char* param3 = strtok(NULL, "|");
    char* param4 = strtok(NULL, "|");
    char* param5 = strtok(NULL, "|");
    char* param6 = strtok(NULL, "|");

    if (command == nullptr) {
        const char* response = "Invalid request format";
        send(communication_socket, response, strlen(response), 0);
        log("Received invalid request.", "ERROR");
        return;
    }

    if (strcmp(command, "LOGIN") == 0) {
        if (param1 == nullptr || param2 == nullptr) {
            const char* response = "Invalid request format";
            send(communication_socket, response, strlen(response), 0);
            log("Received invalid request.", "ERROR");
            return;
        }
        const char* email = param1;
        const char* password = param2;

        int user_ID = authenticate_user(db, email, password);
        std::string response = std::to_string(user_ID);
        send(communication_socket, response.c_str(), response.length(), 0);

    } else if (strcmp(command, "REGISTER") == 0) {
        if (param1 == nullptr || param2 == nullptr || param3 == nullptr) {
            const char* response = "Invalid request format";
            send(communication_socket, response, strlen(response), 0);
            log("Received invalid request.", "ERROR");
            return;
        }
        const char* email = param1;
        const char* password = param2;
        const char* username = param3;
        if (db_register_user(db, email, password, username) == 1) {
            const char* response = "1";
            send(communication_socket, response, strlen(response), 0);
        } else {
            const char* response = "0";
            send(communication_socket, response, strlen(response), 0);
        }
    } else if (strcmp(command, "GAMESTART") == 0) {
        if (param1 == nullptr) {
            const char* response = "Invalid request format";
            send(communication_socket, response, strlen(response), 0);
            log("Received invalid request.", "ERROR");
            return;
        }
        int user_id = stoi(param1);
        update_user_socket_mapping(client_id, user_id);
        add_to_matchmaking_queue(user_id);
    } else if (strcmp(command, "GAMEFINISH") == 0) {
        remove_from_matchmaking_queue(client_id);
        auto it = find_if(pairs.begin(), pairs.end(), [client_id](const pair<int, int>& p) {
            return p.first == client_id || p.second == client_id;
        });

        if (it != pairs.end()) {
            int enemy_id = (it->first == client_id) ? it->second : it->first;
            pairs.erase(it);

            if (clients.find(enemy_id) != clients.end()) {
                const char* message = "ENEMY_DISCONNECTED";
                send(clients[enemy_id], message, strlen(message), 0);
            }
        }
    } else if (strcmp(command, "SINGLEGAMEFINISH") == 0) {
        if (param1 == nullptr || param2 == nullptr || param3 == nullptr || param4 == nullptr || param5 == nullptr || param6 == nullptr) {
            const char* response = "Invalid request format";
            send(communication_socket, response, strlen(response), 0);
            log("Received invalid request.", "ERROR");
            return;
        }
        char* start_datetime = param1;
        char* end_datetime = param2;
        int score = stoi(param3);
        int lines_cleared = stoi(param4);
        int level = stoi(param5);
        int user_id = stoi(param6);
        if (insert_singleplayer_game(db, start_datetime, end_datetime, score, lines_cleared, level, user_id) == 1) {
            const char* response = "1";
            send(communication_socket, response, strlen(response), 0);
        } else {
            const char* response = "0";
            send(communication_socket, response, strlen(response), 0);
        }
    } else if (strcmp(command, "GAME_UPDATE") == 0) {
        if (param1 == nullptr) {
            const char* response = "Invalid request format";
            send(communication_socket, response, strlen(response), 0);
            log("Received invalid request.", "ERROR");
            return;
        }
        char* matrix = param1;
        string message = "UPDATE|" + string(matrix);
        log("Received game update from user_id: " + to_string(socket_to_user[client_id]) + ", message: " + message, "INFO");
        forward_to_enemy(socket_to_user[client_id], message);
    } else if (strcmp(command, "GARBAGE") == 0) {
        if (param1 == nullptr) {
            const char* response = "Invalid request format";
            send(communication_socket, response, strlen(response), 0);
            log("Received invalid request.", "ERROR");
            return;
        }
        int lines = stoi(param1);
        string message = "GARBAGE|" + to_string(lines);
        forward_to_enemy(socket_to_user[client_id], message);
    } else {
        const char* response = "Unknown command";
        send(communication_socket, response, strlen(response), 0);
        cout << command << endl;
        log("Received unknown command.", "ERROR");
    }
}


void client_handler(SOCKET communication_socket, int client_id) {
    char receive_buffer[512] = {0};

    while (true) {
        memset(receive_buffer, 0, sizeof(receive_buffer));
        
        int bytes_received = recv(communication_socket, receive_buffer, sizeof(receive_buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            log("Receive failed: " + to_string(WSAGetLastError()), "ERROR");
            break;
        } else if (bytes_received == 0) {
            log("Client disconnected.", "INFO");
            break;
        }

        receive_buffer[bytes_received] = '\0';
        process_request(receive_buffer, communication_socket, client_id);
    }

    lock_guard<mutex> guard(clients_mutex);

    clients.erase(client_id);
    auto it = find_if(pairs.begin(), pairs.end(), [client_id](const pair<int, int>& p) {
        return p.first == client_id || p.second == client_id;
    });

    if (it != pairs.end()) {
        int enemy_id = (it->first == client_id) ? it->second : it->first;
        pairs.erase(it);

        if (clients.find(enemy_id) != clients.end()) {
            pairs.push_back({enemy_id, -1});
        }
    }

    if (socket_to_user.find(client_id) != socket_to_user.end()) {
        int user_id = socket_to_user[client_id];
        socket_to_user.erase(client_id);
        user_to_socket.erase(user_id);
    }

    closesocket(communication_socket);
}


int main() {
    WSADATA wsa_data;
    SOCKET connection_socket, communication_socket;
    struct sockaddr_in server_address, client_address;

    logFile.open("server_log.log", ios::out | ios::app);
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

    if (!open_database("DataBase.db")) {
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

        lock_guard<mutex> guard(clients_mutex);
        int client_id = static_cast<int>(communication_socket); 
        clients[client_id] = communication_socket;

        thread client_thread(client_handler, communication_socket, client_id);
        client_thread.detach();
    }

    closesocket(connection_socket);
    WSACleanup();
    logFile.close();
    free_config_parameters(config);
    return 0;
}

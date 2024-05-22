#include "client.hpp"
#include "config_file_parser.h"
#include "menus.h"
#include "tetris/multiplayer_tetris_game.hpp"
#include <atomic>
#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>

using namespace std;

#define DEFAULT_SERVER_IP "127.0.0.1" // Default values if config file fails
#define DEFAULT_SERVER_PORT 6000
extern "C" void menus_create_multiplayer_game(SOCKET sock);

int user_id = 0;
int block_menu_loop = 0;

MultiplayerTetrisGame *game;

ofstream logFile("client_log.log", ios::app);
atomic<bool> keep_running(false);
thread listener_thread;

void log(const string &message, const string &level) {
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

void send_request(SOCKET sock, const char *command, const char *email, const char *password, const char *username) {
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
        log("recv failed: " + std::to_string(WSAGetLastError()), "ERROR");
        return -1;
    }

    server_reply[recv_size] = '\0';

    log("Server reply: " + std::string(server_reply), "INFO");

    try {
        int result = std::stoi(server_reply);

        if (result == 0) {
            std::cout << "Operacion fallida, intentalo de nuevo." << std::endl;
        } else if (result > 0) {
            std::cout << "Operacion exitosa" << std::endl;
            user_id = result;
        }

        return result;
    } catch (const std::invalid_argument &e) {
        log("Invalid response received, not a number: " + std::string(server_reply), "ERROR");
        return -1;
    } catch (const std::out_of_range &e) {
        log("Number out of range in response: " + std::string(server_reply), "ERROR");
        return -1;
    }
}

void handle_match_found(SOCKET sock) {
    log("Match found, initializing game", "INFO");
    game = new MultiplayerTetrisGame(sock);
    block_menu_loop = 0;
}

void listen_for_updates(SOCKET sock) {
    char receive_buffer[512];
    fd_set readfds;
    struct timeval tv;

    u_long mode = 1; // Non blocking mode
    ioctlsocket(sock, FIONBIO, &mode);

    while (keep_running) {

        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        tv.tv_sec = 0;
        tv.tv_usec = 10;

        int activity = select(0, &readfds, NULL, NULL, &tv);

        if (activity == SOCKET_ERROR) {
            log("Select failed: " + to_string(WSAGetLastError()), "ERROR");
            break;
        }

        if (activity > 0 && FD_ISSET(sock, &readfds)) {
            int bytes_received = recv(sock, receive_buffer, sizeof(receive_buffer) - 1, 0);
            if (bytes_received == SOCKET_ERROR) {
                log("Receive failed: " + to_string(WSAGetLastError()), "ERROR");
                break;
            } else if (bytes_received == 0) {
                log("Server disconnected.", "INFO");
                break;
            }

            receive_buffer[bytes_received] = '\0';
            string received_message(receive_buffer);

            if (received_message.rfind("UPDATE|", 0) == 0) {
                string matrix = received_message.substr(7);
                log("Update from server (matrix): " + matrix, "INFO");
                // if (game != nullptr) game->updateEnemyPlayfield(matrix);
            } else if (received_message.rfind("GARBAGE|", 0) == 0) {
                int lines = stoi(received_message.substr(8));
                log("Garbage lines received from server: " + to_string(lines), "INFO");
                // if (game != nullptr) game->enqueueGarbage(lines);
            } else if (received_message.rfind("MATCHED|", 0) == 0) {
                cout << "Partida encontrada" << endl;
                log("Match found from server", "INFO");
                std::thread game_thread(handle_match_found, sock);
                game_thread.detach();
            } else {
                cout << "Unknown message from server: " << received_message << endl;
                log("Unknown message from server: " + received_message, "INFO");
            }
        }
    }

    mode = 0; // Blocking mode
    ioctlsocket(sock, FIONBIO, &mode);
}

void send_game_update(SOCKET sock, const string &matrix) {
    string message = "GAME_UPDATE|" + matrix;
    if (send(sock, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        log("Send failed: " + to_string(WSAGetLastError()), "ERROR");
    }
}

void send_garbage(SOCKET sock, int lines) {
    string message = "GARBAGE|" + to_string(lines);
    if (send(sock, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        log("Send failed: " + to_string(WSAGetLastError()), "ERROR");
    }
}

void send_game_start(SOCKET sock, int user_id) {
    string message = "GAMESTART|" + to_string(user_id);
    if (send(sock, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Send GAMESTART failed: " << WSAGetLastError() << std::endl;
        log("Send GAMESTART failed: " + to_string(WSAGetLastError()), "ERROR");
    }
}

void send_game_finish(SOCKET sock) {
    string message = "GAMEFINISH";
    if (send(sock, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Send GAMEFINISH failed: " << WSAGetLastError() << std::endl;
        log("Send GAMEFINISH failed: " + to_string(WSAGetLastError()), "ERROR");
    }
}

void send_singleplayer_game_finish(SOCKET sock, const char *start_datetime, const char *finish_datetime, int score, int linesCleared, int level) {
    char message[256];
    sprintf(message, "SINGLEGAMEFINISH|%s|%s|%d|%d|%d|%d", start_datetime, finish_datetime, score, linesCleared, level, user_id);
    if (send(sock, message, strlen(message), 0) < 0) {
        std::cerr << "Send SINGLEGAMEFINISH failed: " << WSAGetLastError() << std::endl;
        log("Send SINGLEGAMEFINISH failed: " + to_string(WSAGetLastError()), "ERROR");
    }
}

void send_multiplayer_game_finish(SOCKET sock, int id_user1, int id_user2, bool first_win, const char *start_datetime, const char *finish_datetime) {
    char message[256];
    sprintf(message, "MULTIPLAYERGAMEFINISH|%d|%d|%d|%s|%s", id_user1, id_user2, first_win, start_datetime, finish_datetime);
    if (send(sock, message, strlen(message), 0) < 0) {
        std::cerr << "Send MULTIPLAYERGAMEFINISH failed: " << WSAGetLastError() << std::endl;
        log("Send MULTIPLAYERGAMEFINISH failed: " + to_string(WSAGetLastError()), "ERROR");
    }
}

void start_listener_thread(SOCKET sock) {
    keep_running = true;
    listener_thread = thread(listen_for_updates, sock);
}

void stop_listener_thread() {
    keep_running = false;
    if (listener_thread.joinable()) {
        listener_thread.join();
    }
}

void start_client() {
    log("Client application started", "INFO");
    SOCKET sock = connect_to_server();
    if (sock != INVALID_SOCKET) {
        display_menu(sock);

        stop_listener_thread();

        closesocket(sock);
        log("Socket closed", "INFO");
        WSACleanup();
        log("Winsock cleanup complete", "INFO");
    }
    log("Client application ended", "INFO");
}

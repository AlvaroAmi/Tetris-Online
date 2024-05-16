#include <iostream>
#include <thread>
#include <winsock2.h>
#include <string>
#include <atomic>
#include <chrono>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

std::atomic<bool> keep_running(true);
std::atomic<bool> matched(false);

void log(const std::string& message) {
    std::cout << message << std::endl;
}

void listen_for_updates(SOCKET sock) {
    char receive_buffer[512];
    while (keep_running) {
        int bytes_received = recv(sock, receive_buffer, sizeof(receive_buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            log("Receive failed: " + std::to_string(WSAGetLastError()));
            break;
        } else if (bytes_received == 0) {
            log("Server disconnected.");
            break;
        }

        receive_buffer[bytes_received] = '\0';
        std::string response(receive_buffer);
        std::cout << "Client1 received: " << response << std::endl;

        if (response.find("MATCHED") != std::string::npos) {
            matched = true;
        }
    }
}

int main() {
    WSADATA wsa_data;
    SOCKET sock;
    struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "Failed to initialize Winsock: " << WSAGetLastError() << std::endl;
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "Connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    std::thread listener_thread(listen_for_updates, sock);

    std::string start_message = "GAMESTART";
    send(sock, start_message.c_str(), start_message.length(), 0);

    while (!matched) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Send two matrices
    std::string matrix1 = "GAME_UPDATE|matrix1 from Client1";
    send(sock, matrix1.c_str(), matrix1.length(), 0);
    std::this_thread::sleep_for(std::chrono::seconds(2)); 
    std::string matrix2 = "GAME_UPDATE|matrix2 from Client1";
    send(sock, matrix2.c_str(), matrix2.length(), 0);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::string finish_message = "GAMEFINISH";
    send(sock, finish_message.c_str(), finish_message.length(), 0);

    keep_running = false;
    listener_thread.join();

    closesocket(sock);
    WSACleanup();
    return 0;
}

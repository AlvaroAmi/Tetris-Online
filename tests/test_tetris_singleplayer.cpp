#include "tetris/singleplayer_tetris_game.hpp"

int main(int argc, char const *argv[]) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    SingleplayerTetrisGame game(sock);
    return 0;
}

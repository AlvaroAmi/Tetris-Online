#include "tetris/multiplayer_tetris_game.hpp"

int main(int argc, char const *argv[]) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    MultiplayerTetrisGame game(sock);
    game.enqueueGarbage(1);
    return 0;
}

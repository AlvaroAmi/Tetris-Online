#include <string>
#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif
#include "playfield.hpp"

class EnemyPlayerDisplay {
private:
    Playfield playfield;

public:
    EnemyPlayerDisplay();
    void draw();
    void updateGame(std::string &matrixString);
};
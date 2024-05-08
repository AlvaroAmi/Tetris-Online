#ifndef PLAYFIELD_HPP
#define PLAYFIELD_HPP

#define PLAYFIELD_WIDTH 10
#define PLAYFIELD_HEIGHT 40
#define VISIBLE_PLAYFIELD_HEIGHT 20

#include "tetris/tetromino.hpp"

class Playfield {
private:
    Color (*matrix)[PLAYFIELD_WIDTH] = new Color[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH]();

    bool isLineFull(int line) const;

    void clearLine(int line);
    bool isTileEmpty(int x, int y) const;
    bool isOutOfBounds(int x, int y) const;

public:
    Color getTile(int x, int y) const;

    void setTile(int x, int y, Color color);

    int attemptClearLines();

    bool tetrominoFits(const Tetromino &tetromino) const;

    void lockTetromino(const Tetromino &tetromino);

    Color (*getMatrix())[PLAYFIELD_WIDTH];
};

#endif
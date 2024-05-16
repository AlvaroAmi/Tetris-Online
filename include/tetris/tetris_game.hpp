#ifndef TETRIS_GAME_HPP
#define TETRIS_GAME_HPP

#include "tetris/vector2.hpp"

#define MAX_DEFINED_LEVEL 29

class TetrisGame {
protected:
    static constexpr Vector2<int> BASE_SPAWN_POSITION = Vector2<int>(3, 19);
    static constexpr Vector2<int> LEFT_VECTOR = Vector2<int>(-1, 0);
    static constexpr Vector2<int> RIGHT_VECTOR = Vector2<int>(1, 0);
    static constexpr Vector2<int> DOWN_VECTOR = Vector2<int>(0, -1);
    static constexpr int GRAVITY[MAX_DEFINED_LEVEL + 1] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};
    virtual void updateGame() = 0;
};

#endif // TETRIS_GAME_HPP
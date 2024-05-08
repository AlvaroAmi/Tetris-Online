#ifndef BAG_RANDOM_GENERATOR_HPP
#define BAG_RANDOM_GENERATOR_HPP

#include "tetris/tetromino.hpp"
#include <algorithm>
#include <random>
#include <vector>

class BagRandomGenerator {
private:
    std::vector<TetrominoType> bag;
    std::mt19937 rng;

    void resetBag() {
        bag = {TetrominoType::I, TetrominoType::J, TetrominoType::L, TetrominoType::O, TetrominoType::S, TetrominoType::T, TetrominoType::Z};
        std::shuffle(bag.begin(), bag.end(), rng);
    }

public:
    BagRandomGenerator() {
        rng.seed(std::random_device()());
        resetBag();
    }

    TetrominoType getNextTetrominoType() {
        if (bag.empty()) resetBag();

        TetrominoType nextTetromino = bag.back();
        bag.pop_back();
        return nextTetromino;
    }
};

#endif
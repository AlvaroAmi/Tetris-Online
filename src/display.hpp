#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#if defined(_WIN32) || defined(_WIN64)
#define NCURSES_STATIC
#include <ncurses/ncurses.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#else
#error "Unsupported operating system"
#endif

#include "tetris/tetromino.hpp"
#include <stdlib.h>
#include <iostream>
using namespace std;

void init_colors(void);
int paint_matrix(WINDOW *win, int (*matrix)[10]);
#endif
#ifndef MENU_H
#define MENU_H

void print_tetris_logo();

void clear_if_needed(char *str, int max_line);

char main_menu();

char** log_in_menu(char *option);

char offline_menu();

void log_in();

void play_offline();

bool is_valid(char *mail, char *password);

void register_user();


#endif
#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

int insert_user(sqlite3 *db, const char *username, const char *email, const char *password, int highest_score);
int delete_user(sqlite3 *db, const char *username);
int show_user(sqlite3 *db, const char *username, const char *password);

#endif // DATABASE_H

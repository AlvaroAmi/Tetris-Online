#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

int insert_user(sqlite3 *db, const char *username, const char *email, const char *password, int highestScore);
int delete_user(sqlite3 *db, const char *username);

#endif // DATABASE_H

#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

int insertUser(sqlite3 *db, const char *username, const char *email, const char *password, int highestScore);
int deleteUser(sqlite3 *db, const char *username);

#endif // DATABASE_H

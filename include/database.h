#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int authenticate_user(sqlite3* db, const char* email, const char* password);
int db_register_user(sqlite3* db, const char* email, const char* password, const char* username);
int delete_user(sqlite3* db, const char* email);
char* get_username(sqlite3* db, int user_id);

#ifdef __cplusplus
}
#endif

#endif // DATABASE_H

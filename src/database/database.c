#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <pthread.h>
#include <string.h>
#include "database.h"

int authenticate_user(sqlite3* db, const char* email, const char* password) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT 1 FROM USER WHERE email = ? AND password = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing login statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return result == SQLITE_ROW;
}

int db_register_user(sqlite3* db, const char* email, const char* password, const char* username) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO USER (email, password, username) VALUES (?, ?, ?);";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing registration statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, username, -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return result == SQLITE_DONE;
}

int delete_user(sqlite3* db, const char* email) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM USER WHERE email = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing delete statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return result == SQLITE_DONE;
}

char* get_username(sqlite3* db, int user_id) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT username FROM USER WHERE id = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing get username statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    const char* username = (const char*)sqlite3_column_text(stmt, 0);
    char* usernameCopy = (char*)malloc(strlen(username) + 1);
    strcpy(usernameCopy, username);

    sqlite3_finalize(stmt);
    return usernameCopy;
}

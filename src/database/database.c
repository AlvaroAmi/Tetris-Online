#include "database.h"
#include "sqlite3.h"
#include <stdio.h>
#include <string.h>

int insert_user(sqlite3 *db, const char *username, const char *email, const char *password, int highest_score) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO USER (username, email, password, highestScore) VALUES (?, ?, ?, ?);";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, password, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, highest_score);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error al insertar nuevo usuario\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        printf("Error al finalizar statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    return SQLITE_OK;
}

int show_user(sqlite3 *db, const char *email, const char *password) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT password, highestScore FROM USER WHERE email = ?;";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando statement (SELECT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);

    return result == SQLITE_ROW ? SQLITE_OK : result;
}

int delete_user(sqlite3 *db, const char *username) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM USER WHERE username = ?;";

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error al preparar statement (DELETE)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error al borrar user\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        printf("Error al finalizar statement (DELETE)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Usuario eliminado exitosamente\n");

    return SQLITE_OK;
}

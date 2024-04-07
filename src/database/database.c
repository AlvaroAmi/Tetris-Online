#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "../../include/database.h"

int insertUser(sqlite3 *db, const char *username, const char *email, const char *password, int highestScore) {
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
    sqlite3_bind_int(stmt, 4, highestScore);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error al insertar nuevo usuario\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt); 
        return result;
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        printf("Error fal finalizar statement (INSERT)\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Nuevo usuario insertado exitosamente\n");

    return SQLITE_OK;
}

int deleteUser(sqlite3 *db, const char *username) {
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

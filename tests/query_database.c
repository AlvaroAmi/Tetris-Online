#include <stdio.h>
#include "sqlite3.h"

void select_data(sqlite3 *db, char *query);

int main() {
    sqlite3 *db;
    char *err_msg = 0;

    // Open DataBase
    int rc = sqlite3_open("TetrisOnline.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stdout, "Base de datos abierta exitosamente\n");
    }

    // Select and display data from User
    char *select_users = "SELECT * FROM User;";
    select_data(db, select_users);

    // Select and display data from Multiplayer_Game
    char *select_multiplayer_games = "SELECT * FROM Multiplayer_Game;";
    select_data(db, select_multiplayer_games);

    // Select and display data from Singleplayer_Game
    char *select_singleplayer_games = "SELECT * FROM Singleplayer_Game;";
    select_data(db, select_singleplayer_games);

    // Close the database
    sqlite3_close(db);

    return 0;
}

void select_data(sqlite3 *db, char *query) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparando la consulta SQL: %s\n", sqlite3_errmsg(db));
        return;
    }

    int cols = sqlite3_column_count(stmt);
    int result = 0;

    while (1) {
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW) {
            for (int col = 0; col < cols; col++) {
                const char *val = (const char*)sqlite3_column_text(stmt, col);
                printf("%s = %s\t", sqlite3_column_name(stmt, col), val ? val : "NULL");
            }
            printf("\n");
        } else if (result == SQLITE_DONE) {
            printf("Seleccion completada\n");
            break;
        } else {
            printf("Error ejecutando la consulta SQL: %s\n", sqlite3_errmsg(db));
            break;
        }
    }

    sqlite3_finalize(stmt);
}
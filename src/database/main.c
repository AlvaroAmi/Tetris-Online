#include <stdio.h>
#include "sqlite3.h"
#include "../../include/database.h"

int main() {
    sqlite3 *db;
    int result = sqlite3_open("TetrisOnline.db", &db); // Reemplaza "your_database.db" con el nombre real de tu archivo de base de datos
    if (result != SQLITE_OK) {
        printf("Error opening database\n");
        return result;
    }

    // Insertar usuario
    result = insertUser(db, "new_username", "new_user@example.com", "new_password", 0);
    if (result != SQLITE_OK) {
        printf("Error inserting new user\n");
        sqlite3_close(db);
        return result;
    }

    // Eliminar usuario
    result = deleteUser(db, "old_username");
    if (result != SQLITE_OK) {
        printf("Error deleting user\n");
        sqlite3_close(db);
        return result;
    }

    // Aquí podrías añadir más operaciones si es necesario.

    result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        printf("Error closing database\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Database closed\n");

    return 0;
}

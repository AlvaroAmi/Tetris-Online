#include <stdio.h>
#include "sqlite3.h"
#include "database.h"

int main() {
    sqlite3 *db;
    int result = sqlite3_open("TetrisOnline.db", &db); 
    if (result != SQLITE_OK) {
        printf("Error al abrir base de datos\n");
        return result;
    }

    // Insertar usuario
    result = insert_user(db, "new_username", "new_user@example.com", "new_password", 0);
    if (result != SQLITE_OK) {
        printf("Error al insertar nuevo usuario\n");
        sqlite3_close(db);
        return result;
    }

    // Eliminar usuario
    result = delete_user(db, "old_username");
    if (result != SQLITE_OK) {
        printf("Error al borrar usuario\n");
        sqlite3_close(db);
        return result;
    }

    result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        printf("Error al cerrar base de datos\n");
        printf("%s\n", sqlite3_errmsg(db));
        return result;
    }

    printf("Base de datos cerrada\n");

    return 0;
}

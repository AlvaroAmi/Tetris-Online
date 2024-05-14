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

    // Registrar usuario
    result = db_register_user(db, "new_user@example.com", "new_password", "new_username");
    if (result != SQLITE_DONE) {
        printf("Error al registrar nuevo usuario\n");
        sqlite3_close(db);
        return result;
    } else {
        printf("Nuevo usuario registrado exitosamente\n");
    }

    // Autenticar usuario
    result = authenticate_user(db, "new_user@example.com", "new_password");
    if (result == 1) {
        printf("Usuario autenticado exitosamente\n");
    } else {
        printf("Error al autenticar usuario\n");
    }

    // Eliminar usuario
    result = delete_user(db, "new_user@example.com");
    if (result != SQLITE_DONE) {
        printf("Error al borrar usuario\n");
        sqlite3_close(db);
        return result;
    } else {
        printf("Usuario borrado exitosamente\n");
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

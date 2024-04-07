#include <stdio.h>
#include "sqlite3.h"

void create_table(int rc, sqlite3 *db, char *query, char *table_name, char *err_msg);

int main() {
    sqlite3 *db;
    char *err_msg = 0;
    char *test_users[5];


    // Crate or Open DataBase
    int rc = sqlite3_open("TetrisOnline.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stdout, "Base de datos abierta exitosamente\n");
    }

    // Creation of tables(Queue, User, Complete Game, Duel)
    char *create_queue = "CREATE TABLE Queue (id INTEGER PRIMARY KEY, inicio DATETIME, mode TEXT, FOREIGN KEY (id) REFERENCES User(id));";
    char *create_user = "CREATE TABLE User (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, email TEXT, password TEXT, highestScore INT DEFAULT 0);";
    char *create_complete_game = "CREATE TABLE Complete_Game (id INTEGER PRIMARY KEY AUTOINCREMENT, score INTEGER, idUser INTEGER, mode TEXT, timestamp DATETIME, FOREIGN KEY (idUser) REFERENCES User(id));";
    char *create_duel = "CREATE TABLE Duel (id INTEGER PRIMARY KEY AUTOINCREMENT, idUser1 INTEGER, idUser2 INTEGER, firstWins BOOLEAN, ended BOOLEAN, timestamp DATETIME, mode TEXT, FOREIGN KEY (idUser1) REFERENCES User(id), FOREIGN KEY (idUser2) REFERENCES User(id));";

    create_table(rc, db, create_queue, "Queue", err_msg);
    create_table(rc, db, create_user, "User", err_msg);
    create_table(rc, db, create_complete_game, "Complete_Game", err_msg);
    create_table(rc, db, create_duel, "Duel", err_msg);

    // User addition for test
    char *create_user1 = "INSERT INTO User (username, email, password) values ('@alvaro', 'alvaro@gmail.com', '12345');";
    char *create_user2 = "INSERT INTO User (username, email, password) values ('@irune', 'irune@gmail.com', '23456');";
    char *create_user3 = "INSERT INTO User (username, email, password) values ('@jaime', 'jaime@gmail.com', '34567');";
    char *create_user4 = "INSERT INTO User (username, email, password) values ('@erik', 'erik@gmail.com', '45678');";
    char *create_user5 = "INSERT INTO User (username, email, password) values ('@aimar', 'aimar@gmail.com', '56789');";

    rc = sqlite3_exec(db, create_user1, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error ejecutando la consulta SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Tabla creada correctamente \n");
    }
    rc = sqlite3_exec(db, create_user2, 0, 0, &err_msg);
    rc = sqlite3_exec(db, create_user3, 0, 0, &err_msg);
    rc = sqlite3_exec(db, create_user4, 0, 0, &err_msg);
    rc = sqlite3_exec(db, create_user5, 0, 0, &err_msg);

    // Cerrar la base de datos
    sqlite3_close(db);

    return 0;
}

void create_table(int rc, sqlite3 *db, char *query, char *table_name, char *err_msg) {
    rc = sqlite3_exec(db, query, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error ejecutando la consulta SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Tabla %s creada correctamente \n", table_name);
    }
}
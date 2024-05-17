#include <stdio.h>
#include "sqlite3.h"

void create_table(int rc, sqlite3 *db, char *query, char *table_name, char *err_msg);
void insert_data(int rc, sqlite3 *db, char *query, char *table_name, char *err_msg);

int main() {
    sqlite3 *db;
    char *err_msg = 0;

    // Crate or Open DataBase
    int rc = sqlite3_open("TetrisOnline.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stdout, "Base de datos abierta exitosamente\n");
    }

    // Creation of tables (User, Multiplayer_Game, Singleplayer_Game)
    char *create_user = "CREATE TABLE User (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, email TEXT, password TEXT, highestScore INT DEFAULT 0);";
    char *create_multiplayer_game = "CREATE TABLE Multiplayer_Game (id INTEGER PRIMARY KEY AUTOINCREMENT, idUser1 INTEGER, idUser2 INTEGER, firstWins BOOLEAN, start DATETIME, end DATETIME, FOREIGN KEY (idUser1) REFERENCES User(id), FOREIGN KEY (idUser2) REFERENCES User(id));";
    char *create_singleplayer_game = "CREATE TABLE Singleplayer_Game (id INTEGER PRIMARY KEY AUTOINCREMENT, start_datetime DATETIME, finish_datetime DATETIME, score INTEGER, linesCleared INTEGER, level INTEGER, player_id INTEGER, FOREIGN KEY (player_id) REFERENCES User(id));";

    create_table(rc, db, create_user, "User", err_msg);
    create_table(rc, db, create_multiplayer_game, "Multiplayer_Game", err_msg);
    create_table(rc, db, create_singleplayer_game, "Singleplayer_Game", err_msg);

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
        fprintf(stdout, "Usuario creado correctamente \n");
    }
    rc = sqlite3_exec(db, create_user2, 0, 0, &err_msg);
    rc = sqlite3_exec(db, create_user3, 0, 0, &err_msg);
    rc = sqlite3_exec(db, create_user4, 0, 0, &err_msg);
    rc = sqlite3_exec(db, create_user5, 0, 0, &err_msg);

    // Insert sample data into Multiplayer_Game
    char *insert_multiplayer_game1 = "INSERT INTO Multiplayer_Game (idUser1, idUser2, firstWins, start, end) VALUES (1, 2, 1, '2024-05-01 10:00:00', '2024-05-01 10:30:00');";
    char *insert_multiplayer_game2 = "INSERT INTO Multiplayer_Game (idUser1, idUser2, firstWins, start, end) VALUES (3, 4, 0, '2024-05-02 14:00:00', '2024-05-02 14:45:00');";
    
    insert_data(rc, db, insert_multiplayer_game1, "Multiplayer_Game", err_msg);
    insert_data(rc, db, insert_multiplayer_game2, "Multiplayer_Game", err_msg);

    // Insert sample data into Singleplayer_Game
    char *insert_singleplayer_game1 = "INSERT INTO Singleplayer_Game (start_datetime, finish_datetime, score, linesCleared, level, player_id) VALUES ('2024-05-03 12:00:00', '2024-05-03 12:45:00', 1200, 50, 10, 1);";
    char *insert_singleplayer_game2 = "INSERT INTO Singleplayer_Game (start_datetime, finish_datetime, score, linesCleared, level, player_id) VALUES ('2024-05-04 15:00:00', '2024-05-04 15:30:00', 800, 30, 8, 2);";
    
    insert_data(rc, db, insert_singleplayer_game1, "Singleplayer_Game", err_msg);
    insert_data(rc, db, insert_singleplayer_game2, "Singleplayer_Game", err_msg);

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

void insert_data(int rc, sqlite3 *db, char *query, char *table_name, char *err_msg) {
    rc = sqlite3_exec(db, query, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error insertando datos en la tabla %s: %s\n", table_name, err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Datos insertados correctamente en la tabla %s \n", table_name);
    }
}
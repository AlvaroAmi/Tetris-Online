#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "database.h"

// Declaración de funciones
void create_table(int rc, sqlite3 *db, char *query, char *table_name, char *err_msg);
void insert_data(int rc, sqlite3 *db, char *query, char *table_name, char *err_msg);
char* calculate_game_duration(sqlite3 *db, int id, int is_multiplayer);  

int main() {
    sqlite3 *db;
    char *err_msg = 0;

    // Crate or Open DataBase
    int rc = sqlite3_open("DataBase.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stdout, "Base de datos abierta exitosamente\n");
    }

    // Creation of tables (User, Multiplayer_Game, Singleplayer_Game, Game_Sessions)
    char *create_user = "CREATE TABLE User (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, email TEXT, password TEXT, highestScore INT DEFAULT 0);";
    char *create_multiplayer_game = "CREATE TABLE Multiplayer_Game (id INTEGER PRIMARY KEY AUTOINCREMENT, idUser1 INTEGER, idUser2 INTEGER, firstWins BOOLEAN, start DATETIME, end DATETIME, FOREIGN KEY (idUser1) REFERENCES User(id), FOREIGN KEY (idUser2) REFERENCES User(id));";
    char *create_singleplayer_game = "CREATE TABLE Singleplayer_Game (id INTEGER PRIMARY KEY AUTOINCREMENT, start_datetime DATETIME, finish_datetime DATETIME, score INTEGER, linesCleared INTEGER, level INTEGER, player_id INTEGER, FOREIGN KEY (player_id) REFERENCES User(id));";
    char *create_game_sessions = "CREATE TABLE Game_Sessions (id INTEGER PRIMARY KEY AUTOINCREMENT, idUser INTEGER, game_id INTEGER, game_duration TEXT, FOREIGN KEY (idUser) REFERENCES User(id));";

    create_table(rc, db, create_user, "User", err_msg);
    create_table(rc, db, create_multiplayer_game, "Multiplayer_Game", err_msg);
    create_table(rc, db, create_singleplayer_game, "Singleplayer_Game", err_msg);
    create_table(rc, db, create_game_sessions, "Game_Sessions", err_msg);

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

    // Calculate and insert game duration into Game_Sessions
    int game_id = 1;  
    for (int i = 1; i <= 2; i++) {
        char *duration = calculate_game_duration(db, game_id, 1);  
        if (duration != NULL) {
            char query[256];
            snprintf(query, sizeof(query), "INSERT INTO Game_Sessions (idUser, game_id, game_duration) VALUES (%d, %d, '%s');", i, game_id, duration);
            insert_data(rc, db, query, "Game_Sessions", err_msg);
            free(duration);
        }
        game_id++;
    }
    for (int i = 1; i <= 2; i++) {
        char *duration = calculate_game_duration(db, game_id, 0);  
        if (duration != NULL) {
            char query[256];
            snprintf(query, sizeof(query), "INSERT INTO Game_Sessions (idUser, game_id, game_duration) VALUES (%d, %d, '%s');", i, game_id, duration);
            insert_data(rc, db, query, "Game_Sessions", err_msg);
            free(duration);
        }
        game_id++;
    }

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

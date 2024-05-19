#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "database.h"

void select_data(sqlite3 *db, char *query);

int main() {
    sqlite3 *db;
    char *err_msg = 0;

    // Open DataBase
    int rc = sqlite3_open("DataBase.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        fprintf(stdout, "Database opened successfully\n");
    }

    // Test user registration
    if (db_register_user(db, "testuser@example.com", "password123", "testuser")) {
        printf("User registered successfully.\n");
    } else {
        printf("Failed to register user.\n");
    }

    // Test user authentication
    int user_id = authenticate_user(db, "testuser@example.com", "password123");
    if (user_id > 0) {
        printf("User authenticated successfully. User ID: %d\n", user_id);
    } else {
        printf("Failed to authenticate user.\n");
    }

    // Test getting username
    char *username = get_username(db, user_id);
    if (username) {
        printf("Username: %s\n", username);
        free(username);  // Free the allocated memory
    } else {
        printf("User not found or error occurred.\n");
    }


    // Test calculating game duration
    char *multiplayer_duration = calculate_game_duration(db, 1, 1);
    if (multiplayer_duration) {
        printf("Multiplayer game duration: %s\n", multiplayer_duration);
        free(multiplayer_duration);
    } else {
        printf("Failed to calculate multiplayer game duration.\n");
    }

    char *singleplayer_duration = calculate_game_duration(db, 1, 0);
    if (singleplayer_duration) {
        printf("Singleplayer game duration: %s\n", singleplayer_duration);
        free(singleplayer_duration);
    } else {
        printf("Failed to calculate singleplayer game duration.\n");
    }

    // Test user deletion
    if (delete_user(db, "testuser@example.com")) {
        printf("User deleted successfully.\n");
    } else {
        printf("Failed to delete user.\n");
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
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
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
            printf("Selection completed\n");
            break;
        } else {
            printf("Error executing SQL statement: %s\n", sqlite3_errmsg(db));
            break;
        }
    }

    sqlite3_finalize(stmt);
}

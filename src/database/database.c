#include "database.h"
#include <pthread.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define strptime for Windows
#ifdef _WIN32
char* strptime(const char* s, const char* f, struct tm* tm);
int strptime_helper(const char* s, const char* f, struct tm* tm, char** res);

char* strptime(const char* s, const char* f, struct tm* tm) {
    // Use a temp variable to hold time string
    char* res = (char*)s;
    if (!strptime_helper(s, f, tm, &res)) {
        return NULL;
    }
    return res;
}

int strptime_helper(const char* s, const char* f, struct tm* tm, char** res) {
    while (*f != '\0') {
        if (*f == '%') {
            f++;
            if (*f == 'Y') {
                sscanf(s, "%4d", &tm->tm_year);
                tm->tm_year -= 1900;
                s += 4;
            } else if (*f == 'm') {
                sscanf(s, "%2d", &tm->tm_mon);
                tm->tm_mon--;
                s += 2;
            } else if (*f == 'd') {
                sscanf(s, "%2d", &tm->tm_mday);
                s += 2;
            } else if (*f == 'H') {
                sscanf(s, "%2d", &tm->tm_hour);
                s += 2;
            } else if (*f == 'M') {
                sscanf(s, "%2d", &tm->tm_min);
                s += 2;
            } else if (*f == 'S') {
                sscanf(s, "%2d", &tm->tm_sec);
                s += 2;
            } else {
                return 0; // Unknown format
            }
        } else {
            if (*s != *f) {
                return 0; // Mismatch
            }
            s++;
        }
        f++;
    }
    *res = (char*)s;
    return 1;
}
#endif

// Returns 0 in case of failure, user id otherwise
int authenticate_user(sqlite3 *db, const char *email, const char *password) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM USER WHERE email = ? AND password = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing login statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 0;
    }

    int userId = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    return userId;
}

int db_register_user(sqlite3 *db, const char *email, const char *password, const char *username) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO USER (email, password, username) VALUES (?, ?, ?);";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing registration statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, username, -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return result == SQLITE_DONE;
}

int delete_user(sqlite3 *db, const char *email) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM USER WHERE email = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing delete statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return result == SQLITE_DONE;
}

char *get_username(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT username FROM USER WHERE id = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing get username statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    const char *username = (const char *)sqlite3_column_text(stmt, 0);
    char *usernameCopy = (char *)malloc(strlen(username) + 1);
    strcpy(usernameCopy, username);

    sqlite3_finalize(stmt);
    return usernameCopy;
}

int insert_multiplayer_game(sqlite3 *db, int idUser1, int idUser2, int firstWins, const char *start, const char *end){
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Multiplayer_Game (idUser1, idUser2, firstWins, start, end) VALUES (?, ?, ?, ?, ?);";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing insert multiplayer game statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, idUser1);
    sqlite3_bind_int(stmt, 2, idUser2);
    sqlite3_bind_int(stmt, 3, firstWins);
    sqlite3_bind_text(stmt, 4, start, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, end, -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return result == SQLITE_DONE;
}

int insert_singleplayer_game(sqlite3 *db, const char *start_datetime, const char *finish_datetime, int score, int linesCleared, int level, int player_id){
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Singleplayer_Game (start_datetime, finish_datetime, score, linesCleared, level, player_id) VALUES (?, ?, ?, ?, ?, ?);";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing insert singleplayer game statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, start_datetime, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, finish_datetime, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, score);
    sqlite3_bind_int(stmt, 4, linesCleared);
    sqlite3_bind_int(stmt, 5, level);
    sqlite3_bind_int(stmt, 6, player_id);

    result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return result == SQLITE_DONE;
}

char *calculate_game_duration(sqlite3 *db, int id, int is_multiplayer) {
    const char *sql;
    if (is_multiplayer) {
        sql = "SELECT start, end FROM Multiplayer_Game WHERE id = ?;";
    } else {
        sql = "SELECT start_datetime, finish_datetime FROM Singleplayer_Game WHERE id = ?;";
    }

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, id);

    char *duration_str = NULL;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *start = (const char *)sqlite3_column_text(stmt, 0);
        const char *end = (const char *)sqlite3_column_text(stmt, 1);

        struct tm tm_start, tm_end;
        time_t t_start, t_end;

        if (strptime(start, "%Y-%m-%d %H:%M:%S", &tm_start) == NULL) {
            fprintf(stderr, "Error parsing start time\n");
            sqlite3_finalize(stmt);
            return NULL;
        }

        if (strptime(end, "%Y-%m-%d %H:%M:%S", &tm_end) == NULL) {
            fprintf(stderr, "Error parsing end time\n");
            sqlite3_finalize(stmt);
            return NULL;
        }

        tm_start.tm_isdst = -1;  
        tm_end.tm_isdst = -1;   

        t_start = mktime(&tm_start);
        t_end = mktime(&tm_end);

        if (t_start == -1 || t_end == -1) {
            fprintf(stderr, "Error converting time to epoch\n");
            sqlite3_finalize(stmt);
            return NULL;
        }

        double duration = difftime(t_end, t_start);

        int hours = (int)(duration / 3600);
        int minutes = (int)((duration - (hours * 3600)) / 60);
        int seconds = (int)(duration - (hours * 3600) - (minutes * 60));

        duration_str = (char *)malloc(9); 
        if (duration_str != NULL) {
            snprintf(duration_str, 9, "%02d:%02d:%02d", hours, minutes, seconds);
        }
    }

    sqlite3_finalize(stmt);
    return duration_str;
}
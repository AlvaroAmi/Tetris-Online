#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <pthread.h>
#include "database.h"

typedef struct QueueNode {
    int user_id;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

Queue matchmakingQueue = {NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

void add_to_queue(int user_id) {
    pthread_mutex_lock(&matchmakingQueue.mutex);

    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->user_id = user_id;
    newNode->next = NULL;

    if (matchmakingQueue.rear == NULL) {
        matchmakingQueue.front = matchmakingQueue.rear = newNode;
    } else {
        matchmakingQueue.rear->next = newNode;
        matchmakingQueue.rear = newNode;
    }
    matchmakingQueue.size++;

    pthread_cond_signal(&matchmakingQueue.cond);
    pthread_mutex_unlock(&matchmakingQueue.mutex);
}

int match_users(int* user1, int* user2) {
    pthread_mutex_lock(&matchmakingQueue.mutex);

    while (matchmakingQueue.size < 2) {
        pthread_cond_wait(&matchmakingQueue.cond, &matchmakingQueue.mutex);
    }

    QueueNode* node1 = matchmakingQueue.front;
    *user1 = node1->user_id;
    matchmakingQueue.front = matchmakingQueue.front->next;

    QueueNode* node2 = matchmakingQueue.front;
    *user2 = node2->user_id;
    matchmakingQueue.front = matchmakingQueue.front->next;

    if (matchmakingQueue.front == NULL) {
        matchmakingQueue.rear = NULL;
    }

    free(node1);
    free(node2);
    matchmakingQueue.size -= 2;

    pthread_mutex_unlock(&matchmakingQueue.mutex);
    return 1;
}

int authenticate_user(sqlite3* db, const char* email, const char* password) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT 1 FROM USER WHERE email = ? AND password = ?;";
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing login statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return result == SQLITE_ROW;
}

int db_register_user(sqlite3* db, const char* email, const char* password, const char* username) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO USER (email, password, username) VALUES (?, ?, ?);";
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

int delete_user(sqlite3* db, const char* email) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM USER WHERE email = ?;";
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

char* get_username(sqlite3* db, int user_id) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT username FROM USER WHERE  id = ?;";
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

    const char* username = (const char*)sqlite3_column_text(stmt, 0);
    char* usernameCopy = (char*)malloc(strlen(username) + 1);
    strcpy(usernameCopy, username);

    sqlite3_finalize(stmt);
    return usernameCopy;
}

int main() {
    sqlite3 *db;
    int rc;

    // Abre la base de datos
    rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (1);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    // Obtén el nombre de usuario
    char *username = get_username(db, 1);
    if (username) {
        printf("Username: %s\n", username);
        free(username);  // Libera la memoria asignada
    } else {
        printf("User not found or error occurred.\n");
    }

    // Cierra la base de datos
    sqlite3_close(db);
    return 0;
}
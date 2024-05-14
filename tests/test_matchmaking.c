#include <stdio.h>
#include <pthread.h>
#include "database.h"

void* user_thread(void* arg) {
    int user_id = *((int*)arg);
    add_to_queue(user_id);
    printf("User %d added to queue\n", user_id);
    return NULL;
}

int main() {
    int user1 = 1;
    int user2 = 2;

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, user_thread, &user1);
    pthread_create(&thread2, NULL, user_thread, &user2);

    int matched_user1, matched_user2;
    match_users(&matched_user1, &matched_user2);

    printf("Matched users: %d and %d\n", matched_user1, matched_user2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5
#define ITERATIONS 10000

sem_t semaphore;
int counter = 0;

void *increment_counter(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        // Wait (decrement) the semaphore before entering the critical section
        sem_wait(&semaphore);
        counter++; // Critical section
        // Post (increment) the semaphore after leaving the critical section
        sem_post(&semaphore);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize the semaphore with an initial value of 1 (binary semaphore)
    if (sem_init(&semaphore, 0, 1) != 0) {
        printf("Semaphore initialization failed.\n");
        return 1;
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            printf("Thread creation failed.\n");
            return 1;
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", counter);

    // Destroy the semaphore
    sem_destroy(&semaphore);

    return 0;
}

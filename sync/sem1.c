// sem1.c
//
// Basic usage of POSIX semaphores.

#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#define N_THREADS (2)
#define N_OPS     (10000)

static size_t count = 0;

void* worker(void *arg) {
    sem_t *lock = (sem_t*)arg;

    for (size_t i = 0; i < N_OPS; ++i) {
        sem_wait(lock);
        ++count;
        sem_post(lock);
    }

    pthread_exit((void*)EXIT_SUCCESS);
}

int main(void) {
    sem_t lock;
    sem_init(&lock, 0, 1);

    pthread_t threads[N_THREADS];
    for (size_t i = 0; i < N_THREADS; ++i) {
        pthread_create(&threads[i], NULL, worker, (void*)&lock);
    }

    for (size_t i = 0; i < N_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("[+] Count: %zu\n", count);
    assert(count == (N_THREADS*N_OPS));

    sem_destroy(&lock);
    return EXIT_SUCCESS;
}
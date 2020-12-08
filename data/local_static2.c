// local_static2.c
//
// Local statics in a multithreaded program.

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define N_THREADS (2)
#define N_ITERS   (5)

pthread_mutex_t lock;

void* do_work(void* arg)
{
	size_t id = (size_t)arg;

	static size_t local = 0;
	for (size_t i = 0; i < N_ITERS; ++i)
	{
		pthread_mutex_lock(&lock);
		printf("[%zu] Local Count = %zu\n", id, local++);
		pthread_mutex_unlock(&lock);
	}

	pthread_exit((void*)EXIT_SUCCESS);
}

int main(void)
{
	pthread_mutex_init(&lock, NULL);

	pthread_t threads[N_THREADS];
	for (size_t i = 0; i < N_THREADS; ++i) 
	{
		pthread_create(&threads[i], NULL, do_work, (void*)i);
	}

	// wait for all threads to complete
	for (size_t i = 0; i < N_THREADS; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&lock);
	return EXIT_SUCCESS;
}
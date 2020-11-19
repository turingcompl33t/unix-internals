// prethreaded1.c
//
// A concurrent echo server that utilizes a pre-allocated pool 
// of worker threads to manage concurrent client connections.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "core.h"
#include "util.h"

// the number of threads allocated to threadpool
#define THREADPOOL_SIZE 2

#define BUFSIZE 256

typedef struct thread_ctx
{
    int             sfd;
    pthread_mutex_t lock;
} thread_ctx_t;

static void handle_connection(int cfd)
{   
    ssize_t n_bytes;
    char buffer[BUFSIZE];
    while ((n_bytes = read(cfd, buffer, BUFSIZE)) > 0)
    {
        if (write(cfd, buffer, n_bytes) != n_bytes)
        {
            pthread_exit((void*)EXIT_FAILURE);
        }
    }

    if (-1 == n_bytes)
    {
        pthread_exit((void*)EXIT_FAILURE);
    }
}

static void* server_worker(void* c)
{
    thread_ctx_t* ctx = (thread_ctx_t*)c;

    int cfd;
    socklen_t addrlen;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));

    for (;;)
    {
        pthread_mutex_lock(&ctx->lock);

        cfd = accept(ctx->sfd, (struct sockaddr*)&addr, &addrlen);
        if (-1 == cfd)
        {
            pthread_exit((void*)EXIT_FAILURE);
        }

        printf("[+] Accepted client connection\n");

        pthread_mutex_unlock(&ctx->lock);

        handle_connection(cfd);
        close(cfd);
    }

    pthread_exit((void*)EXIT_SUCCESS);
}

int main(void)
{
    int sfd = server_socket(SERVER_PORT);
    if (-1 == sfd)
    {
        error_exit("server_socket()");
    }

    // initialize the shared thread context
    thread_ctx_t ctx;
    ctx.sfd = sfd;
    if (pthread_mutex_init(&ctx.lock, NULL) != 0)
    {
        error_exit("pthread_mutex_init()");
    }

    printf("[+] Listening on port %s\n", SERVER_PORT);

    // populate the threadpool
    pthread_t threadpool[THREADPOOL_SIZE];
    for (size_t i = 0; i < THREADPOOL_SIZE; ++i)
    {
        if (pthread_create(&threadpool[i], NULL, server_worker, (void*)&ctx) != 0)
        {
            error_exit("pthread_create()");
        }
    }

    // join all the threads in the pool
    for (size_t i = 0; i < THREADPOOL_SIZE; ++i)
    {
        if (pthread_join(threadpool[i], NULL) != 0)
        {
            error_exit("pthread_join()");
        }
    }

    // cleanup the shared context
    close(ctx.sfd);
    pthread_mutex_destroy(&ctx.lock);

    return EXIT_SUCCESS;
}
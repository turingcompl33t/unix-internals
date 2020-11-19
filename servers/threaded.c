// threaded.c
//
// A concurrent echo server that spawns a 
// new thread for each client connection.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "core.h"
#include "util.h"

#define BUFSIZE 256

// ----------------------------------------------------------------------------
// Dead-Simple Intrusive List

typedef struct list_entry
{   
    struct list_entry* blink;
    struct list_entry* flink;
} list_entry_t;

static void list_initialize_head(list_entry_t* head)
{
    head->blink = head;
    head->flink = head;
}

static bool list_is_empty(list_entry_t* head)
{
    return ((NULL == head->blink) && (NULL == head->flink));
}

static void list_push(list_entry_t* head, list_entry_t* entry)
{
    entry->blink = head;
    entry->flink = head->flink;
    head->flink->blink = entry;
    head->flink = entry;
}

list_entry_t* list_pop(list_entry_t* head)
{
    if (list_is_empty(head))
    {
        return NULL;
    }

    list_entry_t* popped = head->flink;

    head->flink = popped->flink;
    popped->flink->blink = head;

    return popped;
}

// ----------------------------------------------------------------------------
// Server Implementation

typedef void*(*thread_start_fn)(void*);

typedef struct thread
{
    list_entry_t entry;
    pthread_t    thread;
    int          cfd;
} thread_t;

static thread_t* start_thread(thread_start_fn fn, int fd)
{
    thread_t* t = (thread_t*) calloc(1, sizeof(thread_t));
    if (NULL == t)
    {
        error_exit("malloc()");
    }

    t->cfd = fd;
    if (pthread_create(&t->thread, NULL, fn, &t->cfd) != 0)
    {
        error_exit("pthread_create()");
    }

    return t;
}

static void join_thread(thread_t* t)
{
    if (pthread_join(t->thread, NULL) != 0)
    {
        error_exit("pthread_join()");
    }

    free(t);
}

static void* handle_connection(void* ctx)
{
    int cfd = *((int*)ctx);

    ssize_t n_bytes;
    char buffer[BUFSIZE];

    while ((n_bytes = read(cfd, buffer, BUFSIZE)) > 0)
    {
        if (write(cfd, buffer, n_bytes) != n_bytes)
        {
            return NULL;
        }
    }

    close(cfd);
    return NULL;
}

int main(void)
{
    int sfd = server_socket(SERVER_PORT);
    if (-1 == sfd)
    {
        error_exit("server_socket()");
    }

    list_entry_t threads;
    list_initialize_head(&threads);

    int cfd;
    socklen_t addrlen;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));

    printf("[+] Listening on port %s\n", SERVER_PORT);

    for (;;)
    {
        // accept a new client connection
        cfd = accept(sfd, (struct sockaddr*)&addr, &addrlen);
        if (-1 == cfd)
        {
            error_exit("accept()");
        }

        printf("[+] Spawning thread to handle new connection\n");
    
        thread_t* t = start_thread(handle_connection, cfd);
        list_push(&threads, &t->entry);
    }

    // join with all of the worker threads
    while (!list_is_empty(&threads))
    {
        thread_t* t = (thread_t*)list_pop(&threads);
        join_thread(t);
    }

    close(sfd);
    return EXIT_SUCCESS;
}
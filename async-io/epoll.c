// epoll.c
// Basic epoll example.
//
// Build
//  gcc -Wall -Werror -std=gnu11 epoll1.c -o epoll1.out -pthread

#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <unistd.h>

typedef struct epoll_event epoll_event_t;

typedef struct ctx
{
    int write_fd;
} ctx_t;

static const size_t N_THREADS      = 2;
static const size_t BYTES_TO_WRITE = 512;
static const size_t BUFSIZE        = 256;

static void error(const char* msg)
{
    printf("[-] %s\n", msg);
    exit(EXIT_FAILURE);
}

static void* reader(void* arg)
{
    ctx_t* ctx = (ctx_t*)arg;
    int write_fd = ctx->write_fd;

    FILE* random = fopen("/dev/urandom", "r");
    if (NULL == random)
    {
        return (void*) EXIT_FAILURE;
    }

    char buffer[BUFSIZE];
    size_t bytes_written = 0;
    while (bytes_written < BYTES_TO_WRITE)
    {
        size_t bytes_read = fread(buffer, 1, BUFSIZE, random);
        ssize_t n = write(write_fd, buffer, bytes_read);
        if (-1 == n)
        {
            return (void*) EXIT_FAILURE;
        }
        bytes_written += n;
    }

    fclose(random);
    close(write_fd);

    return (void*) EXIT_SUCCESS;
}

int main(void)
{
    int       pipes[2*N_THREADS];
    pthread_t threads[N_THREADS];

    ctx_t contexts[N_THREADS];

    epoll_event_t ev;
    epoll_event_t events[N_THREADS];

    int instance = epoll_create1(0);
    if (-1 == instance)
    {
        error("epoll_create1()");
    }

    for (size_t i = 0; i < N_THREADS; ++i)
    {
        if (-1 == pipe(&pipes[i*2]))
        {
            error("pipe()");
        }

        const int read_pipe  = pipes[i*2];
        const int write_pipe = pipes[i*2 + 1];

        contexts[i].write_fd = write_pipe;

        printf("read pipe: %d\n", read_pipe);
        printf("write pipe: %d\n", write_pipe);

        ev.data.fd = read_pipe;
        ev.events  = EPOLLIN;

        if (-1 == epoll_ctl(instance, EPOLL_CTL_ADD, read_pipe, &ev))
        {
            error("epoll_ctl()");
        }

        if (pthread_create(&threads[i], 
            NULL, reader, (void*)&contexts[i]) != 0)
        {
            error("pthread_create()");
        }
    }

    char buffer[BUFSIZE];

    size_t n_remaining = N_THREADS;
    while (n_remaining > 0)
    {
        int n_ready = epoll_wait(instance, events, N_THREADS, -1);
        if (-1 == n_ready)
        {
            if (EINTR == errno)
            {
                continue;
            }
            else
            {
                error("epoll_wait()");
            }
        }

        // deal with each ready fd
        for (int j = 0; j < n_ready; ++j)
        {
            printf("fd: %d events: %s%s%s\n", 
                events[j].data.fd, 
                (events[j].events & EPOLLIN) ? "EPOLLIN " : "",
                (events[j].events & EPOLLHUP) ? "EPOLLHUP " : "",
                (events[j].events & EPOLLERR) ? "EPOLLERR " : "");
            
            if (events[j].events & EPOLLIN)
            {
                // data available to be read on pipe
                int s = read(events[j].data.fd, buffer, BUFSIZE);
                if (-1 == s)
                {
                    error("read()");
                }

                printf("\tread %d bytes\n", s);
            }
            else if (events[j].events & (EPOLLHUP | EPOLLERR))
            {
                printf("\tclosing fd %d\n", events[j].data.fd);
                close(events[j].data.fd);
                n_remaining--;
            }
        }
    }

    void* ret;
    for (size_t i = 0; i < N_THREADS; ++i)
    {
        pthread_join(threads[i], &ret);
    }

    return EXIT_SUCCESS;
}
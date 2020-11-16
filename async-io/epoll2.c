// epoll2.c
//
// Can we poll stdin with epoll_wait()?
//
// Answer: apparently yes.

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/epoll.h>

#include "core.h"

#define BUFLEN 256

int main(void)
{   
    // disable buffering on stdout
    setbuf(stdout, NULL);

    // set stdin to nonblocking

    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (-1 == flags)
    {
        error_exit("fcntl()");
    }

    flags |= O_NONBLOCK;
    if (-1 == fcntl(STDIN_FILENO, F_SETFL, flags))
    {
        error_exit("fcntl()");
    }

    int pollfd = epoll_create1(0);
    if (-1 == pollfd)
    {
        error_exit("epoll_create()");
    }

    // add stdin to epoll instance

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;

    if (-1 == epoll_ctl(pollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev))
    {
        error_exit("epoll_ctl()");
    }

    char buffer[BUFLEN];
    memset(buffer, 0, BUFLEN);

    fprintf(stdout, "> ");
    for (;;)
    {
        int n_events = epoll_wait(pollfd, &ev, 1, -1);
        if (-1 == n_events)
        {
            error_exit("epoll_wait()");
        }

        ssize_t bytes_read = read(STDIN_FILENO, buffer, BUFLEN);
        if (-1 == bytes_read)
        {
            error_exit("read()");
        }

        // remove trailing newline
        char *newline = strchr(buffer, '\n');
        if (newline != NULL) {
            *newline = '\0';
            bytes_read -= 1;
        }

        fprintf(stdout, "Read %ld bytes [%s]", bytes_read, buffer);
        fprintf(stdout, "\n");
        fprintf(stdout, "> ");
    }

    return EXIT_SUCCESS;
}
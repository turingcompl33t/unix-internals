// eventfd.c
//
// Multiplexing arbitrary event notification with eventfd and epoll.

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#define DEFAULT_N_REPS 5

void error_exit(const char* msg)
{
    printf("[-] %s (%d)\n", msg, errno);
    exit(EXIT_FAILURE);
}

void post(const int event)
{
    uint64_t buffer = 1;

    // writing to the event increments the internal count
    if (write(event, &buffer, sizeof(buffer)) == -1)
    {
        error_exit("write()");
    }
}

void reactor(const int ioc, const int n_reps)
{
    uint64_t buffer = 0;
    struct epoll_event ev;

    for (int i = 0; i < n_reps; ++i)
    {
        int const n_events = epoll_wait(ioc, &ev, 1, -1);
        if (-1 == n_events)
        {
            break;
        }

        // read from the event to decrement the internal count 
        read(ev.data.fd, &buffer, sizeof(buffer));

        puts("[+] event posted");
    }
}

int main(int argc, char* argv[])
{
    // yes, I realize atoi() is suboptimal
    int const n_reps = (argc > 1) 
        ? atoi(argv[1]) 
        : DEFAULT_N_REPS;

    int const ioc = epoll_create1(0);
    if (-1 == ioc)
    {
        error_exit("epoll_create1()");
    }

    int const event = eventfd(0, EFD_SEMAPHORE);
    if (-1 == event)
    {
        error_exit("eventfd()");
    }

    struct epoll_event ev;
    ev.events   = EPOLLIN;
    ev.data.fd  = event;
    if (epoll_ctl(ioc, EPOLL_CTL_ADD, event, &ev) == -1)
    {
        error_exit("epoll_ctl()");
    }

    for (int i = 0; i < n_reps; ++i)
    {
        post(event);
    }

    reactor(ioc, n_reps);

    close(event);
    close(ioc);

    return EXIT_SUCCESS;
}
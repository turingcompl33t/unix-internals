// signalfd3.c
//
// Combining signalfd with epoll and EPOLLONESHOT.

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/epoll.h>
#include <sys/signalfd.h>

#include "core.h"

int main(int argc, char* argv[])
{
    printf("[+] %s (%ld)\n", argv[0], (long)getpid());

    // block our signals of interest
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    if (-1 == sigprocmask(SIG_BLOCK, &mask, NULL))
    {
        error_exit("sigprocmask()");
    }

    // create a signalfd instance
    int sfd = signalfd(-1, &mask, O_NONBLOCK);
    if (-1 == sfd) 
    {
        error_exit("signalfd()");
    }

    // create our epoll instance
    int pollfd = epoll_create1(0);
    if (-1 == pollfd) 
    {
        error_exit("epoll_create1()");
    }

    // add our signalfd descriptor to the epoll instance
    struct epoll_event ev;
    ev.events  = EPOLLIN | EPOLLONESHOT;
    ev.data.fd = sfd;

    if (-1 == epoll_ctl(pollfd, EPOLL_CTL_ADD, sfd, &ev))
    {
        error_exit("epoll_ctl()");
    }

    // poll

    struct signalfd_siginfo info;
    for (;;)
    {
        int n_events = epoll_wait(pollfd, &ev, 1, -1);
        if (n_events != 1)
        {
            error_exit("epoll_wait()");
        }

        ssize_t n_bytes = read(sfd, &info, sizeof(info));
        if (-1 == n_bytes)
        {
            error_exit("read()");
        }

        printf("[+] Got signal: %d\n", info.ssi_signo);

        ev.events = EPOLLIN | EPOLLONESHOT;
        if (-1 == epoll_ctl(pollfd, EPOLL_CTL_MOD, sfd, &ev))
        {
            error_exit("epoll_ctl()");
        }
    }

    close(sfd);
    close(pollfd);

    return EXIT_SUCCESS;
}
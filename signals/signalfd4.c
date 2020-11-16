// signalfd4.c
//
// Refusing to receive one signal until another is delivered.
//
// Summary: saw strange errors when attempting to disable and
// subsequently re-enable monitoring for different signals on
// distinct signalfd instances; the epoll instance would report 
// that the signal was pending but then the nonblocking read
// would subsequently fail with EAGAIN.

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/epoll.h>
#include <sys/signalfd.h>

#include "core.h"

int make_sfd(int signo)
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, signo);

    int sfd = signalfd(-1, &mask, O_NONBLOCK);
    if (-1 == sfd)
    {
        error_exit("signalfd");
    }

    return sfd;
}

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

    // create a signalfd instance for each signal
    int sfd_u1 = make_sfd(SIGUSR1);
    int sfd_u2 = make_sfd(SIGUSR2);

    // create an epoll instance
    int pollfd = epoll_create1(0);
    if (-1 == pollfd)
    {
        error_exit("epoll_create1");
    }

    // add our signal descriptors to the epoll instance

    struct epoll_event ev;
    ev.events = EPOLLIN;

    ev.data.fd = sfd_u1;
    if (-1 == epoll_ctl(pollfd, EPOLL_CTL_ADD, sfd_u1, &ev))
    {
        error_exit("epoll_ctl()");
    }

    ev.data.fd = sfd_u2;
    if (-1 == epoll_ctl(pollfd, EPOLL_CTL_ADD, sfd_u2, &ev))
    {
        error_exit("epoll_ctl()");
    }

    // poll
    struct signalfd_siginfo info;
    for (;;)
    {
        int n_events = epoll_wait(pollfd, &ev, 1, -1);
        if (-1 == n_events)
        {
            error_exit("epoll_wait()");
        }

        ssize_t n_bytes = read(ev.data.fd, &info, sizeof(info));
        if (-1 == n_bytes)
        {
            error_exit("read()");
        }

        printf("[+] Got signal: %d\n", info.ssi_signo);
    }

    close(sfd_u1);
    close(sfd_u2);
    close(pollfd);
}
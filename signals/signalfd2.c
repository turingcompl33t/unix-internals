// signalfd2.c
//
// Polling different signal masks on different file descriptors.

#include <stdio.h>
#include <stdlib.h>

#include <sys/epoll.h>

#include <sys/signalfd.h>
#include <signal.h>

#include "core.h"

typedef struct epoll_event epoll_event_t;

// make a new signal FD for the specified signal number
int make_signalfd(int signo)
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, signo);
    if (-1 == sigprocmask(SIG_BLOCK, &mask, NULL))
    {
        error_exit("sigprocmask()");
    }

    int sfd = signalfd(-1, &mask, 0);
    if (-1 == sfd)
    {
        error_exit("signalfd()");
    }

    return sfd;
}

int main(int argc, char* argv[]) {
    printf("[+] %s (%ld)\n", argv[0], (long)getpid());

    int sfd_u1 = make_signalfd(SIGUSR1);
    int sfd_u2 = make_signalfd(SIGUSR2);

    // create an epoll instance

    int pollfd = epoll_create1(0);
    if (-1 == pollfd)
    {
        error_exit("epoll_create1()");
    }

    // register the signal FDs with epoll instance

    epoll_event_t ev;
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

    printf("[+] Waiting for signal: %d\n", SIGUSR1);
    printf("[+] Waiting for signal: %d\n", SIGUSR2);

    // poll forever

    struct signalfd_siginfo info;
    for (;;)
    {
        int n_ready = epoll_wait(pollfd, &ev, 1, -1);
        if (-1 == n_ready)
        {
            error_exit("epoll_wait()");
        }

        ssize_t s = read(ev.data.fd, &info, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo))
        {
            error_exit("read()");
        }

        printf("[+] Got signal %d\n", info.ssi_signo);
    }

    close(pollfd);
    close(sfd_u1);
    close(sfd_u2);

    return EXIT_SUCCESS;
}
// suspend1.c
//
// Waiting for a signal with sigsuspend().

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <signal.h>

#include "core.h"

static volatile sig_atomic_t got_usr2 = 0;

static void handler(int signo)
{
    printf("[+] Caught signal %d\n", signo);
    if (SIGUSR2 == signo)
    {
        got_usr2 = 1;
    }
}

int main(int argc, char* argv[]) 
{
    printf("[+] %s (%ld)\n", argv[0], (long)getpid());

    sigset_t original_mask;
    sigset_t blocked_mask;

    // block SIGUSR1 and SIGUSR2

    sigemptyset(&blocked_mask);
    sigaddset(&blocked_mask, SIGUSR1);
    sigaddset(&blocked_mask, SIGUSR2);
    if (-1 == sigprocmask(SIG_BLOCK, &blocked_mask, &original_mask))
    {
        error_exit("sigprocmask()");
    }

    // register handlers for SIGUSR1 and SIGUSR2

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (-1 == sigaction(SIGUSR1, &sa, NULL))
    {
        error_exit("sigaction()");
    }
    if (-1 == sigaction(SIGUSR2, &sa, NULL))
    {
        error_exit("sigaction()");
    }

    for (;!got_usr2;)
    {
        // atomically unblock SIGUSR1 and SIGUSR2;
        // wait for one of the signals to be sent
        if (-1 == sigsuspend(&original_mask) && errno != EINTR)
        {
            error_exit("sigsuspend()");
        }
    }

    return EXIT_SUCCESS;
}
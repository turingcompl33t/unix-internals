// pending1.c
//
// Querying pending signals with sigpending().

#include <stdio.h>
#include <stdlib.h>

#include <sys/signal.h>
#include <signal.h>

#include "core.h"

int main(int argc, char* argv[])
{   
    printf("[+] %s (%ld)\n", argv[0], (long)getpid());

    // block SIGUSR1 and SIGUSR2
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    if (-1 == sigprocmask(SIG_BLOCK, &mask, NULL))
    {
        error_exit("sigprocmask()");
    }


    for (;;)
    {
        printf("[+] ENTER to query pending signals\n");
        getchar();

        sigpending(&mask);
        if (sigismember(&mask, SIGUSR1))
        {
            printf("[+] SIGUSR1 pending\n");
        }
        if (sigismember(&mask, SIGUSR2))
        {
            printf("[+] SIGUSR2 pending\n");
        }
    }

    return EXIT_SUCCESS;
}
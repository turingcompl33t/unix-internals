// spin1.c
//
// Spin until timeout.

#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "core.h"

#define DEFAULT_TIMEOUT 10

static void sigalrm_handler(int signo)
{
    _exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    printf("%s (%ld)\n", argv[0], (long)getpid());

    int timeout = DEFAULT_TIMEOUT;
    if (argc > 1) 
    {
        timeout = atoi(argv[1]);
    }

    // setup handler for sigalrm
    struct sigaction sa;
    sa.sa_handler = sigalrm_handler;
    if (-1 == sigaction(SIGALRM, &sa, NULL))
    {
        error_exit("sigaction");
    }

    // schedule sigalrm
    alarm(timeout);

    // spin
    for (;;);

    // not reached
    return EXIT_SUCCESS;
}
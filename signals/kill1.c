// kill1.c
//
// Demonstration of the futility of handling SIGKILL.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/signal.h>

static void sigkill_handler(int signo)
{
    printf("[+] Caught SIGKILL\n");
}

int main(void)
{
    signal(SIGKILL, sigkill_handler);

    printf("[+] Waiting for SIGKILL...\n");
    for (;;)
    {
        pause();
    }

    return EXIT_SUCCESS;
}
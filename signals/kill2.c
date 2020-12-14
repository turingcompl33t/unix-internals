// kill1.c
//
// Demonstration of the futility of ignoring SIGKILL.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/signal.h>

int main(void)
{
    signal(SIGKILL, SIG_IGN);

    printf("[+] Can't SIGKILL me...\n");
    for (;;)
    {
        pause();
    }

    return EXIT_SUCCESS;
}
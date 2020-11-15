// child1.c
//
// Monitoring child process events with SIGCHLD.

#include <stdio.h>
#include <stdlib.h>

static void sigchld_handler(int signo)
{
    return;
}

int main(void)
{
    return EXIT_SUCCESS;
}
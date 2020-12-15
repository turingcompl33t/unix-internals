// environ.c
//
// Manually examining the process environment.

#define _GNU_SOURCE
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    for (char** ep = environ; *ep != NULL; ++ep)
    {
        puts(*ep);
    }

    return EXIT_SUCCESS;
}
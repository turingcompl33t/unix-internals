// buffering3.c
//
// stdio output buffers are copied to child on fork();
// both buffers will be flushed on call to exit().

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "core.h"

int main(void) {
    // write to the stdio output buffer
    printf("Hello World");
    if (-1 == fork()) 
    {
        error_exit("fork()");
    }

    // both parent and child continue here
    exit(EXIT_SUCCESS);
}

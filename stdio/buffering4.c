// buffering4.c
//
// stdio output to terminal is line-buffered while
// stdio output to file is block-buffered.
//
// Compare output of:
//  ./buffering4
// 
// and 
//  ./buffering4 > out.txt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "core.h"

int main(void) {
    // write to the stdio output buffer
    printf("Hello World\n");
    if (-1 == fork()) 
    {
        error_exit("fork()");
    }

    // both parent and child continue here
    exit(EXIT_SUCCESS);
}

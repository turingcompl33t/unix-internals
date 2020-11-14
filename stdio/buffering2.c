// buffering2.c
//
// stdio line-oriented output buffer is not flushed on _exit().

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    printf("Hello World");
    _exit(EXIT_SUCCESS);
}

// buffering1.c
//
// stdio line-oriented output buffer is flushed on exit().

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Hello World");
    exit(EXIT_SUCCESS);
}

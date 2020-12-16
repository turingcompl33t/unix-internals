// stack1.c
//
// Exploring how data is stored on the runtime stack.

#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 16

typedef unsigned char byte_t;

void call_me(void) {
    byte_t buffer[BUFSIZE];
    for (size_t i = 0; i < BUFSIZE; ++i) {
        buffer[i] = i;
    }

    for (size_t i = 0; i < BUFSIZE; ++i) {
        printf("%p : 0x%02X\n", &buffer[i], buffer[i]);
    }
}

int main(void) {
    call_me();
    return EXIT_SUCCESS;
}
// stack2.c
//
// Exploring how data is stored on the runtime stack.

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte_t;

void call_me(void) {
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    printf("a %p : %d\n", (void*)&a, a);
    printf("b %p : %d\n", (void*)&b, b);
    printf("c %p : %d\n", (void*)&c, c);
    printf("d %p : %d\n", (void*)&d, d);
}

int main(void) {
    call_me();
    return EXIT_SUCCESS;
}
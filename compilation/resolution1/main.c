// main.c
//
// Exploring symbol resolution.

#include <stdio.h>
#include <stdlib.h>

void foo(void);

int a        = 1;
static int b = 2;
int c        = 3;

int main(void)
{
    int c = 4;

    foo();
    printf("a = %d, b = %d, c = %d\n", a, b, c);

    return EXIT_SUCCESS;
}
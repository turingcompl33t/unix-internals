// truncation.c
//
// Signed integral type truncation.

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // an instance that will not retain its sign
    signed short x1 = (short) 0x8070;
    signed char  x2 = (signed char)x1;

    printf("x1 = %d\n", x1);
    printf("x2 = %d\n", x2);

    // an instance that will retain its sign
    signed short y1 = (short)0xFFFF;
    signed char  y2 = (signed char)y1;

    printf("y1 = %d\n", y1);
    printf("y2 = %d\n", y2);

    return EXIT_SUCCESS; 
}

// abs_tmin.c

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void)
{
    const int min = INT_MIN;
    const int res = abs(min);

    printf("INT_MIN      = %d\n", min);
    printf("abs(INT_MIN) = %d\n", res);

    return 0; 
}
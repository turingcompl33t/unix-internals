// struct2.c
//
// Examining structure alignment.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte_t;

typedef struct type1 {
    long a;
    char b;
} type1_t;

typedef struct type2 {
    char a;
    long b;
} type2_t;

void dump_struct(void *s, size_t size) {
    byte_t* ptr = (byte_t*)s;
    for (size_t i = 0; i < size; i+=8) {
        for (size_t j = i; j < (i + 8) && j < size; ++j) {
            printf("0x%02X ", ptr[j]);
        }
        puts("");
    }
    fflush(stdout);
}

int main(void) {
    type1_t t1;
    type2_t t2;

    memset(&t1, 0, sizeof(t1));
    memset(&t2, 0, sizeof(t2));

    t1.a = 1;
    t1.b = 2;
    t2.a = 1;
    t2.b = 2;

    // external padding
    printf("sizeof(t1) = %zu\n", sizeof(t1));
    dump_struct(&t1, sizeof(t1));

    // internal padding
    printf("sizeof(t2) = %zu\n", sizeof(t2));
    dump_struct(&t2, sizeof(t2));

    return EXIT_SUCCESS;
}
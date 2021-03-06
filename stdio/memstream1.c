// memstream1.c
//
// Basic usage of Standard IO memory streams.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"

#define BUFSIZE (48)

int main(void) {
    FILE* fp;
    char buffer[BUFSIZE];

    memset(buffer, 'a', BUFSIZE - 2);
    buffer[BUFSIZE - 2] = '\0';
    buffer[BUFSIZE - 1] = 'X';

    if ((fp = fmemopen(buffer, BUFSIZE, "w+")) == NULL) {
        error_exit("fmemopen()");
        return EXIT_FAILURE;
    }

    // fmemopen() places null at buffer beginning
    printf("initial buffer contents: %s\n", buffer);
    fprintf(fp, "hello world");
    // buffer is unchanged before flush
    printf("before flush: %s\n", buffer);
    fflush(fp);
    // flush modifies the buffer
    printf("after flush: %s\n", buffer);
    printf("len of string in buffer = %ld\n", (long)strlen(buffer));

    // overwrite the buffer with bs
    memset(buffer, 'b', BUFSIZE - 2);
    buffer[BUFSIZE - 2] = '\0';
    buffer[BUFSIZE - 1] = 'X';
    fprintf(fp, "hello world");
    fseek(fp, 0, SEEK_SET);
    // fseek() causes stream flush
    printf("after seek: %s\n", buffer);
    printf("len of string in buffer = %ld\n", (long)strlen(buffer));

    // overwrite the buffer with cs
    memset(buffer, 'c', BUFSIZE - 2);
    buffer[BUFSIZE - 2] = '\0';
    buffer[BUFSIZE - 1] = 'X';
    fprintf(fp, "hello world");
    fclose(fp);
    printf("after close: %s\n", buffer);
    printf("len of string in buffer = %ld\n", (long)strlen(buffer));

    return EXIT_SUCCESS;
}

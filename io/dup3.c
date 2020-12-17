// dup3.c
//
// Does a dup()ed file descriptor increment the reference 
// count for the shared file description? 
//
// Answer: Yes.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "core.h"

#define BUFSIZE  (16)
#define FILENAME "test.txt"

static void write_test_file(const char* filename) {
    char buffer[BUFSIZE];
    
    int fd = open(filename, O_RDWR | O_CREAT, S_IRWXU);
    if (-1 == fd) {
        error_exit("open()");
    }

    memset(buffer, 0, BUFSIZE);
    strncpy(buffer, "abcdefg", BUFSIZE - 1);

    if (write(fd, buffer, strlen(buffer)) != strlen(buffer)) {
        error_exit("write()");
    }

    close(fd);
}

int main(void) {
    write_test_file(FILENAME);

    int fd1 = open(FILENAME, O_RDONLY);
    if (-1 == fd1) {
        error_exit("open()");
    }

    // note: not necessary....
    int fd2 = open(FILENAME, O_RDONLY);
    if (-1 == fd2) {
        error_exit("open()");
    }

    if (-1 == dup2(fd1, fd2)) {
        error_exit("dup2()");
    }

    char c;
    if (read(fd1, &c, 1) != 1) {
        error_exit("read()");
    }

    printf("[+] Read: %c\n", c);
    close(fd1);


    if (read(fd2, &c, 1) != 1) {
        error_exit("read()");
    }

    printf("[+] Read: %c\n", c);
    close(fd2);

    unlink(FILENAME);
    return EXIT_SUCCESS;
}
// open3.c
//
// Behavior with multiple calls to open() on the same file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "core.h"

#define BUFSIZE  (256)
#define FILENAME "test.txt"

void write_test_file(const char* filename)
{
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

int main(void)
{
    write_test_file(FILENAME);

    int fd1 = open(FILENAME, O_RDONLY);
    if (-1 == fd1) {
        error_exit("open()");
    }

    int fd2 = open(FILENAME, O_RDONLY);
    if (-1 == fd2) {
        error_exit("open()");
    }

    char c;
    if (read(fd1, &c, 1) != 1) {
        error_exit("read()");
    }

    printf("[+] Read %c from fd1\n", c);

    if (read(fd2, &c, 1) != 1) {
        error_exit("read()");
    }

    printf("[+] Read %c from fd2\n", c);

    int fd3 = open(FILENAME, O_RDONLY);
    if (-1 == fd3) {
        error_exit("open()");
    }

    if (read(fd3, &c, 1) != 1) {
        error_exit("read()");
    }

    printf("[+] Read %c from fd3\n", c);

    if (read(fd1, &c, 1) != 1) {
        error_exit("read()");
    }

    printf("[+] Read %c from fd1\n", c);

    close(fd1);
    close(fd2);
    close(fd3);
    unlink(FILENAME);

    return EXIT_SUCCESS;    
}
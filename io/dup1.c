// dup.c
//
// File descriptor duplication with dup()

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "core.h"

// 10 chars
char buffer[] = "abcde12345";

int main(void)
{
    int fd1;
    if ((fd1 = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC)) == -1)
    {
        error_exit("open");
    }

    // duplicate the existing fd
    int fd2 = dup(fd1);

    // current position = 0
    assert(lseek(fd1, 0, SEEK_CUR) == 0);
    assert(lseek(fd2, 0, SEEK_CUR) == 0);

    if (write(fd1, buffer, 10) != 10)
    {
        error_exit("write()");
    }

    // current position = 10
    assert(lseek(fd1, 0, SEEK_CUR) == 10);
    assert(lseek(fd2, 0, SEEK_CUR) == 10);

    if (write(fd2, buffer, 10) != 10)
    {
        error_exit("write()");
    }

    // current position = 20
    assert(lseek(fd1, 0, SEEK_CUR) == 20);  
    assert(lseek(fd2, 0, SEEK_CUR) == 20);

    close(fd1);
    close(fd2);

    unlink("test.txt");

    return EXIT_SUCCESS;
}
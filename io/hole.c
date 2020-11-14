// hole.c
//
// Creating a file with a hole using explicit lseek().

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "core.h"

char buffer1[] = "abcde12345";
char buffer2[] = "ABCDE12345";

int main(void)
{
    int fd;
    if ((fd = open("hole.txt", O_WRONLY | O_TRUNC | O_CREAT)) == -1)
    {
        error_exit("open()");
    }

    // file position at 0

    if (write(fd, buffer1, 10) != 10)
    {
        error_exit("write()");
    }

    // file position now at 10

    if (lseek(fd, 16384, SEEK_SET) == -1)
    {
        error_exit("lseek()");
    }

    // file position now 16384

    if (write(fd, buffer2, 10) != 10)
    {
        error_exit("write()");
    }

    // file position now 16394

    off_t current = lseek(fd, 0, SEEK_CUR);
    printf("final offset = %ld\n", (long)current);

    close(fd);
    unlink("hole.txt");

    return EXIT_SUCCESS;
}
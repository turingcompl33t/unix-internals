// share1.c
//
// Sharing of file descriptors between parent and child processes.

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "core.h"

const char* fname   = "test.txt";
const char* content = "abcde";

int main(void) 
{
    int fd;
    if ((fd = open(fname, O_RDWR |O_CREAT)) == -1)
    {
        error_exit("open()");
    }

    // write the content to the file
    const ssize_t expected = (ssize_t) strlen(content);
    if (write(fd, content, expected) != expected)
    {
        error_exit("write()");
    }   

    // seek to the beginning of the file
    if (-1  == lseek(fd, 0, SEEK_SET))
    {
        error_exit("lseek()");
    }

    pid_t p = fork();
    if (-1 == p)
    {
        error_exit("fork()");
    }
    
    if (0 == p)
    {
        // child process
        const off_t n = lseek(fd, 2, SEEK_SET);
        int exit_status = (n == 2) ? EXIT_SUCCESS : EXIT_FAILURE;
        exit(exit_status);
    }

    // parent continues here
    int status;
    if (-1 == waitpid(p, &status, 0))
    {
        error_exit("waitpid()");
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
    {
        error_exit("child terminated abnormally");
    }

    // read the next char from the file
    char buffer;
    if (read(fd, &buffer, 1) != 1)
    {
        error_exit("read()");
    }

    printf("[+] Parent read: %c\n", buffer);

    close(fd);
    unlink(fname);

    return EXIT_SUCCESS;
}
// fork_file_sharing.c
//
// Demo of file-sharing semantics with fork()

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <stdio.h>
#include <unistd.h>

#include "core.h"

int main()
{
    int fd;
    int flags;

    char template[] = "/tmp/testXXXXXX";

    // make stdout non-buffered
    setbuf(stdout, NULL);

    fd = mkstemp(template);
    if (-1 == fd)
    {
        error_exit("mkstemp() failed");
    }

    printf("File offset before fork(): %lld\n",
        (long long)lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
    {
        error_exit("fcntl() failed");
    }

    printf("O_APPEND flag before fork() is: %s\n",
        (flags & O_APPEND) ? "on" : "off");

    switch (fork())
    {
    case -1:
        error_exit("fork() failed");

    case 0:
        // child process
        if (lseek(fd, 1000, SEEK_SET) == -1)
        {
            error_exit("lseek() failed");
        }

        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
        {
            error_exit("fcntl() failed");
        }

        flags |= O_APPEND;
        if (fcntl(fd, F_SETFL, flags) == -1)
        {
            error_exit("fcntl() failed");
        }

        _exit(EXIT_SUCCESS);

    default:
        // parent process

        if (wait(NULL) == -1)
        {
            error_exit("wait() failed");
        }

        printf("Child exited\n");

        printf("File offset in parent: %lld\n",
            (long long)lseek(fd, 0, SEEK_CUR));

        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
        {
            error_exit("fcntl() failed");
        }

        printf("O_APPEND flag in parent is: %s\n",
            (flags & O_APPEND) ? "on": "off");

        exit(EXIT_SUCCESS);
    }
}

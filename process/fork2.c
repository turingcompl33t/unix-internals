// fork2.c
//
// Basic fork and exec.
//
// Build
//  gcc -Wall -Werror -std=gnu11 fork2.c -o fork2.out

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void error(char* msg)
{
    fprintf(stderr, "[-] %s (%d)\n", msg, errno);
}

static void fork_child(int write_fd)
{
    pid_t ret = fork();
    switch (ret)
    {
    case -1:
        error("fork() failed");
        break;
    case 0:
        // child process
        if (dup2(write_fd, STDOUT_FILENO) == -1)
        {
            error("dup2() failed");
            exit(EXIT_FAILURE);
        }

        char* argv[3];
        argv[0] = "echo";
        argv[1] = "hello world";
        argv[2] = NULL;

        execvp(argv[0], argv);
        error("execvp() failed");
        exit(EXIT_FAILURE);

    default:
        // parent process
        printf("[+] forked child process with pid: %d\n", ret);
        break;
    }

    close(write_fd);
}

int main(void)
{
    int pipefds[2];
    const int ret = pipe(pipefds);
    if (-1 == ret)
    {
        error("pipe() failed");
        return EXIT_FAILURE;
    }

    fork_child(pipefds[1]);
    
    int wstatus;
    pid_t pid = wait(&wstatus);

    printf("[+] child process %d exited\n", pid);
    if (WIFEXITED(wstatus))
    {
        printf("[+] status: %u\n", WEXITSTATUS(wstatus));
    }

    char buffer[128];
    size_t bytes_read;
    while ((bytes_read = read(pipefds[0], buffer, 128)) != 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    return EXIT_SUCCESS;
}

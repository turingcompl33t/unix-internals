// reidirect1.c
//
// Basic stdio redirection between child processes.

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static char writer_prog[] = "echo";
static char reader_prog[] = "/usr/bin/wc";

static void error(const char* msg)
{
    fprintf(stderr, "[-] %s (%d)\n", msg, errno);
}

static void fork_writer(int write_fd)
{
    pid_t ret = fork();
    switch (ret)
    {
    case -1:
        error("fork() failed");
        break;
    case 0:
        // child process
        if (-1 == dup2(write_fd, STDOUT_FILENO))
        {
            error("dup2() failed");
            exit(EXIT_FAILURE);
        }

        char* argv_vec[3];
        argv_vec[0] = writer_prog;
        argv_vec[1] = "hello world";
        argv_vec[2] = NULL;

        execvp(argv_vec[0], argv_vec);
        perror("execv() failed for writer");
        exit(EXIT_FAILURE);
        break;
    default:
        // parent process
        printf("[+] forked writer process %d\n", ret);
        break;
    }

    close(write_fd);
}

static void fork_reader(int read_fd, int write_fd)
{
    pid_t ret = fork();
    switch (ret)
    {
    case -1:
        error("fork() failed");
        break;
    case 0:
        // child process
        if (-1 == dup2(read_fd, STDIN_FILENO))
        {
            error("dup2() failed");
            exit(EXIT_FAILURE);
        }

        if (-1 == dup2(write_fd, STDOUT_FILENO))
        {
            error("dup2() failed");
            exit(EXIT_FAILURE);
        }

        char* argv_vec[2];
        argv_vec[0] = reader_prog;
        argv_vec[1] = NULL;

        execv(argv_vec[0], argv_vec);
        perror("execv() failed for reader");
        exit(EXIT_FAILURE);
        break;
    default:
        // parent process
        printf("[+] forked reader process %d\n", ret);
        break;
    }

    close(read_fd);
    close(write_fd);
}

int main(void)
{
    int child_to_child_pipe[2];
    int child_to_parent_pipe[2];

    if (pipe(child_to_child_pipe) == -1)
    {
        error("pipe() failed");
        return EXIT_FAILURE;
    }

    if (pipe(child_to_parent_pipe) == -1)
    {
        error("pipe() failed");
        return EXIT_FAILURE;
    }

    // fork writer and reader processes
    fork_writer(child_to_child_pipe[1]);
    fork_reader(child_to_child_pipe[0], child_to_parent_pipe[1]);

    // wait for children to terminate
    int wstatus;
    for (size_t i = 0; i < 2; ++i)
    {
        pid_t exited = wait(&wstatus);
        printf("[+] child process %d exited\n", exited);
        if (WIFEXITED(wstatus))
        {
            printf("[+] status: %u\n", WEXITSTATUS(wstatus));
        }
    }

    // read the output of the command from child
    char buffer[128];
    size_t bytes_read;
    while ((bytes_read = read(child_to_parent_pipe[0], buffer, 128)) != 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    return EXIT_SUCCESS;
}

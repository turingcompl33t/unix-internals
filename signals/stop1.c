// stop1.c
//
// Stop and and continue target process with kill;
// observe behavior of sigchld signals delivered to
// parent process.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#include "core.h"

static void sigchld_handler(int signo)
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
    {
        if (WIFSTOPPED(status))
        {
            printf("[%ld] Stopped by signal %d\n", (long)pid, WSTOPSIG(status));
        } 
        else if (WIFCONTINUED(status))
        {
            printf("[%ld] Continued\n", (long)pid);
        }
    }
}

static void start_child(void)
{
    pid_t pid = fork();
    if (-1 == pid) 
    {
        error_exit("fork()");
    }

    if (pid != 0)
    {
        // parent
        return;
    }

    char* argv[3];
    argv[0] = "spin1";
    argv[1] = "120";
    argv[2] = NULL;

    if (-1 == execv("./spin1", argv))
    {
        error_exit("execv()");
    }
}

int main(void)
{
    // setup a handler for sigchld
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    if (-1 == sigaction(SIGCHLD, &sa, NULL)) {
        error_exit("sigaction");
    }

    // block sigchld
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    // fork / exec the spin program
    start_child();

    sigemptyset(&mask);
    for (;;)
    {
        if ((-1 == sigsuspend(&mask)) && errno != EINTR) 
        {
            error_exit("sigsuspend()");
        }
    }

    return EXIT_SUCCESS;
}
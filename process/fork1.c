// fork1.c
//
// Basic fork() semantics

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 

static int idata = 111;

int main()
{
    int istack = 222;

    pid_t child_pid;

    switch (child_pid = fork())
    {
    case -1:
        printf("fork() failed\n");
        return EXIT_FAILURE;
    case 0:
        // child process
        idata *= 3;
        istack *= 3;
        break;
    default:
        // parent
        sleep(3);
        break;
    }

    char* id = (child_pid == 0) 
        ? "child" 
        : "parent";

    printf("PID: %ld (%s)\nidata = %d, istack = %d\n", 
        (long)getpid(), id,
        idata, istack);

    return EXIT_SUCCESS;
}

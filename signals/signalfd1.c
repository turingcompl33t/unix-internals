// signalfd1.c
//
// Synchronously waiting for signals with signalfd().

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <sys/signalfd.h>
#include <signal.h>

#include "core.h"

int main(int argc, char *argv[]) {
    if (argc < 2) 
    {
        printf("[-] Usage: %s <SIGNO> [SIGNO ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("[+] %s (%ld)\n", argv[0], (long)getpid());

    // construct the signal mask
    sigset_t mask;
    sigemptyset(&mask);
    for (int j = 1; j < argc; ++j) 
    {
        sigaddset(&mask, atoi(argv[j]));
    }   

    // block the signals specified in the mask
    if (-1 == sigprocmask(SIG_BLOCK, &mask, NULL)) 
    {
        error_exit("sigprocmask()");
    }

    // create the signal file descriptor
    int sfd = signalfd(-1, &mask, 0);
    if (-1 == sfd) 
    {
        error_exit("signalfd()");
    }

    // read signals from the file descriptor
    struct signalfd_siginfo info;
    for (;;)
    {
        ssize_t s = read(sfd, &info, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo))
        {
            error_exit("read()");
        }

        printf("[+] %s got signal %d\n", argv[0], info.ssi_signo);
    }

    return EXIT_SUCCESS;
}
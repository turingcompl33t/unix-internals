// nonlocal1.c
//
// Messing with nonlocal jumps.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <unistd.h>
#include <signal.h>

sigjmp_buf jump_buf;

static void sigint_handler(int signo) {
    siglongjmp(&jump_buf[0], 1);
}

int main(void) {
    if (!sigsetjmp(&jump_buf[0], 1)) {
        signal(SIGINT, sigint_handler);
        printf("[+] Starting %d...\n", getpid());
    } else {
        printf("[+] Restarting...\n");
    }

    for (;;) {
        sleep(2);
        printf("[+] Processing...\n");
    }

    // never reached
    return EXIT_SUCCESS;
}
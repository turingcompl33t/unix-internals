// exec1.c
//
// Recursive exec() program (lol).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (1 == argc) {
        return EXIT_FAILURE;
    }

    char c = (*argv[1])--;
    if (c < 'a') {
        printf("[+] All done\n");
        return EXIT_SUCCESS;
    }

    printf("[+] %s %c %d\n", argv[0], c, getpid());
    execve(argv[0], argv, NULL);

    // not reached
    return EXIT_SUCCESS;
}
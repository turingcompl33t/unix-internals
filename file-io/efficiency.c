// efficiency.c
//
// Quick test of IO efficiency.
//
// Build
//  clang -o efficiency.out -std=c11 -Wall efficiency.c
//
// Test
//  /usr/bin/time -p cat ~/texts/vhl.pdf | ./efficiency.out 1 > /dev/null
//
// NOTE: be wary of successive runs on the same input; the impact 
// of the system cache will likely invalidate the results 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("invalid arguments");
        printf("usage: %s <BUFFER SIZE>\n", argv[0]);
        return 1;
    }

    // zero validation of bufsize
    int bufsize = atoi(argv[1]);
    char* buffer = (char*) malloc(bufsize);

    int n;

    while ((n = read(STDIN_FILENO, buffer, bufsize)) > 0)
    {
        if (write(STDOUT_FILENO, buffer, bufsize) != n)
        {
            printf("write() failed\n");
            break;
        }
    }

    if (n < 0)
    {
        printf("read() error\n");
    }

    free(buffer);

    return 0;
}
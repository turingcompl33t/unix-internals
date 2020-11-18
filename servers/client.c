// client.c
//
// Simple echo server client.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "core.h"
#include "util.h"

#define BUFSIZE 256

int main(void)
{
    // create a client socket
    int cfd = client_socket(SERVER_ADDR, SERVER_PORT);
    if (-1 == cfd)
    {
        error_exit("client_socket()");
    }

    ssize_t n_bytes;
    char buffer[BUFSIZE];

    // read from STDIN, write to socket, read response, write to STDOUT
    while ((n_bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    {
        if (write(cfd, buffer, n_bytes) != n_bytes)
        {
            error_exit("partial write()");
        }

        if (read(cfd, buffer, n_bytes) != n_bytes)
        {
            error_exit("partial read()");
        }

        write(STDOUT_FILENO, buffer, n_bytes);
        write(STDOUT_FILENO, (void*)'\n', 1);
    }

    if (-1 == n_bytes)
    {
        error_exit("read()");
    }

    close(cfd);
    return EXIT_SUCCESS;
}
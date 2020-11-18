// iterative.c
//
// Dead-simple iterative echo server.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "core.h"
#include "util.h"

#define BUFSIZE 256

static void handle_connection(int cfd)
{       
    ssize_t n_bytes;
    char buffer[BUFSIZE];
    while ((n_bytes = read(cfd, buffer, sizeof(buffer))) > 0)
    {
        if (write(cfd, buffer, n_bytes) != n_bytes)
        {
            error_exit("partial write()");
        }
    }
}

int main(void)
{
    int sfd = server_socket(SERVER_PORT);
    if (-1 == sfd)
    {
        error_exit("server_socket()");
    }

    int cfd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    char addr_buffer[INET_ADDRSTRLEN];

    memset(&addr, 0, sizeof(struct sockaddr_in));
    memset(addr_buffer, 0, sizeof(addr_buffer));

    printf("[+] Listening on port %s\n", SERVER_PORT);

    for (;;)
    {
        cfd = accept(sfd, (struct sockaddr*)&addr, &addrlen);
        if (-1 == cfd)
        {
            error_exit("accept()");
        }

        if (inet_ntop(AF_INET, &addr.sin_addr, addr_buffer, sizeof(addr_buffer)) != NULL)
        {
            printf("[+] Handling connection from %s\n", addr_buffer);
        }

        // handle the client connection
        handle_connection(cfd);

        // terminate the connection
        close(cfd);
    }

    close(sfd);
    return EXIT_SUCCESS;
}
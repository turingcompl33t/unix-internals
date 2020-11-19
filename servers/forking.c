// forking.c
//
// Concurrent echo server that forks a child 
// process for each new client connection. 

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "core.h"
#include "util.h"

#define BUFSIZE 256

static void sigchld_handler(int signo)
{
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {}
}

static void handle_connection(int cfd)
{
    ssize_t n_bytes;
    char buffer[BUFSIZE];
    while ((n_bytes = read(cfd, buffer, BUFSIZE)) > 0)
    {
        if (write(cfd, buffer, n_bytes) != n_bytes)
        {
            error_exit("write()");
        }
    }

    if (-1 == n_bytes)
    {
        error_exit("read()");
    }

    close(cfd);
}

int main(void)
{
    // setup handler for SIGCHLD
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    if (-1 == sigaction(SIGCHLD, &sa, NULL))
    {
        error_exit("sigaction()");
    }

    int sfd = server_socket(SERVER_PORT);
    if (-1 == sfd)
    {
        error_exit("server_socket()");
    }

    socklen_t addrlen;
    struct sockaddr_in addr;
    char addr_buffer[INET_ADDRSTRLEN];

    memset(&addr, 0, sizeof(struct sockaddr_in));
    memset(addr_buffer, 0, sizeof(addr_buffer));

    printf("[+] Listening on port %s\n", SERVER_PORT);

    for (;;)
    {
        // accept a new client connection
        int cfd = accept(sfd, (struct sockaddr*)&addr, &addrlen);
        if (-1 == cfd)
        {
            error_exit("accept()");
        }

        if (inet_ntop(AF_INET, &addr.sin_addr, addr_buffer, sizeof(addr_buffer)) != NULL)
        {
            printf("[+] Handling connection from %s\n", addr_buffer);
        }

        // fork a child process to handle the connection
        switch (fork())
        {
        case -1:
            error_exit("fork()");
        case 0:
            // child process
            close(sfd);
            handle_connection(cfd);
            _exit(EXIT_SUCCESS);
        default:
            // parent process
            break;
        }

        // close the client fd (duplicated in child)
        close(cfd);
    }

    close(sfd);
    return EXIT_SUCCESS;
}
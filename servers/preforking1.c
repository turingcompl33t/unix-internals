// preforking1.c
//
// A concurrent echo server with a pre-forked process pool.
//
// In this pre-forking implementation, each child process 
// invokes accept() on the same server file descriptor created
// by the parent process prior to the fork. Under this 
// architecture, the kernel distributes incoming connections 
// among the child processes blocked on accept() equally.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "core.h"
#include "util.h"

// the number of processes in pool
#define PROCESS_POOL_SIZE 2

#define BUFSIZE 256

static void sigchld_handler(int signo)
{   
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {}
}

static void server_loop(const int sfd, const size_t id)
{
    int cfd;
    ssize_t n_bytes;
    char buffer[BUFSIZE];
    
    socklen_t addrlen;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));

    for (;;)
    {
        // wait for a new client connection
        cfd = accept(sfd, (struct sockaddr*)&addr, &addrlen);
        if (-1 == cfd)
        {
            error_exit("accept()");
        }

        printf("[%zu] Handling client connection\n", id);

        // handle the client connection
        while ((n_bytes = read(cfd, buffer, BUFSIZE)) > 0)
        {
            if (write(cfd, buffer, n_bytes) != n_bytes)
            {
                error_exit("partial write()");
            }
        }

        if (-1 == n_bytes)
        {
            error_exit("read()");
        }

        close(cfd);
    }

    // child process closes its copy of the server fd
    close(sfd);
}

int main(void)
{
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

    printf("[+] Listening on port %s\n", SERVER_PORT);

    for (size_t id = 0; id < PROCESS_POOL_SIZE; ++id)
    {
        switch (fork())
        {
        case -1:
            error_exit("fork()");
        case 0:
            server_loop(sfd, id);
            _exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    for (;;)
    {
        pause();
    }

    close(sfd);
    return EXIT_SUCCESS;
}
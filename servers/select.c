// select.c
//
// A concurrent echo server implemented with a select()-based event loop.

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/select.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "core.h"
#include "util.h"

#define BUFSIZE 256

static void set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (-1 == flags)
    {
        error_exit("fcntl()");
    }

    flags |= O_NONBLOCK;
    if (-1 == fcntl(fd, F_SETFL, flags))
    {
        error_exit("fcntl()");
    }
}

static int handle_accept_ready(int fd)
{
    socklen_t addrlen;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));

    int cfd = accept(fd, (struct sockaddr*)&addr, &addrlen);
    if (-1 == cfd)
    {
        error_exit("accept()");
    }

    printf("[+] Accepted new connection\n");

    set_nonblocking(cfd);
    return cfd;
}

static bool handle_read_ready(int fd)
{
    char buffer[BUFSIZE];
    ssize_t n_bytes = read(fd, buffer, BUFSIZE);
    if (-1 == n_bytes)
    {
        error_exit("read()");
    } 
    else if (0 == n_bytes)
    {
        printf("[+] Closing connection\n");
        close(fd);
        return false;
    }

    if (write(fd, buffer, n_bytes) != n_bytes)
    {
        error_exit("write()");
    }

    return true;
}

static int process_ready_descriptors(fd_set* readfds, int nfds, const int sfd)
{
    for (int fd = 0; fd < nfds; ++fd)
    {
        if (!FD_ISSET(fd, readfds))
        {
            continue;
        }

        if (sfd == fd)
        {
            int cfd = handle_accept_ready(fd);
            FD_SET(cfd, readfds);
            FD_SET(sfd, readfds);
            nfds = max(nfds, cfd);
            printf("new nfds: %d\n", nfds);
        }
        else 
        {
            if (handle_read_ready(fd))
            {
                FD_SET(fd, readfds);
            }
        }
    }

    return nfds;
}

int main(void)
{
    int sfd = server_socket(SERVER_PORT);
    if (-1 == sfd)
    {
        error_exit("server_socket()");
    }

    // make the listening socket nonblocking
    set_nonblocking(sfd);

    // nfds must be 1 greater than maximum fd in set
    int nfds = sfd + 1;

    // only interested in read events
    fd_set readfds;
    FD_ZERO(&readfds);

    printf("[+] Listening on port %s\n", SERVER_PORT);
    
    FD_SET(sfd, &readfds);
    for (;;)
    {
        // wait for a read event
        int n_ready = select(nfds, &readfds, NULL, NULL, NULL);
        if (-1 == n_ready)
        {
            error_exit("select()");
        }

        nfds = process_ready_descriptors(&readfds, nfds, sfd);
    }

    close(sfd);
    return EXIT_SUCCESS; 
}
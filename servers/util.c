// util.c
//
// Common socket utilities.

#define _GNU_SOURCE
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "util.h"

int client_socket(const char* address, const char* port)
{
    struct addrinfo hints;
    struct addrinfo* results;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr      = NULL;
    hints.ai_next      = NULL;
    hints.ai_flags     = AI_NUMERICSERV;
    hints.ai_family    = AF_INET;
    hints.ai_socktype  = SOCK_STREAM;

    if (getaddrinfo(address, port, &hints, &results) != 0)
    {
        return -1;
    }

    int cfd;
    struct addrinfo* rp;
    for (rp = results; rp != NULL; rp = rp->ai_next)
    {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (-1 == cfd)
        {
            continue;
        }

        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
        {
            // success
            break;
        }

        close(cfd);
    }

    freeaddrinfo(results);

    return (NULL == rp) ? -1 : cfd;
}

int server_socket(const char* port)
{
    struct addrinfo hints;
    struct addrinfo* results;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr      = NULL;
    hints.ai_next      = NULL;
    hints.ai_flags     = AI_PASSIVE | AI_NUMERICSERV;
    hints.ai_family    = AF_INET;
    hints.ai_socktype  = SOCK_STREAM;

    if (getaddrinfo(NULL, port, &hints, &results) != 0)
    {
        return -1;
    }

    int sfd;
    int optval;
    struct addrinfo* rp;
    for (rp = results; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (-1 == sfd)
        {
            continue;
        }

        if (-1 == setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
        {
            close(sfd);
            return -1;
        }

        if (0 == bind(sfd, rp->ai_addr, rp->ai_addrlen))
        {
            // success
            break;
        }

        close(sfd);
    }

    if (NULL == rp)
    {
        return -1;
    }

    if (-1 == listen(sfd, SERVER_BACKLOG))
    {
        close(sfd);
        return -1;
    }

    freeaddrinfo(results);

    return sfd;
}
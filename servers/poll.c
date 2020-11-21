// poll.c
//
// A concurrent echo server implemented with a poll()-based event loop.
//
// NOTE: I don't mess around with maintaining any metdata regarding the
// current number of descriptors that need to be monitored in the pollfd
// set, which might result in some needless inefficiency. This relies on
// the fact that invalid file descriptors (-1) are ignored in the call to
// poll(), which nicely simplifies the implementation.
//
// Adapted from the implementation provided in CS:APP Chapter 12.2.

#define _GNU_SOURCE
#include <poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "core.h"
#include "util.h"

#define MAX_FDS 16

#define BUFSIZE 256

typedef struct server_ctx
{
    // Number of ready descriptors
    int nready;

    // Set of interest descriptors
    struct pollfd pollfds[MAX_FDS];
} server_ctx_t;

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

static void initialize_server_ctx(server_ctx_t* ctx, int sfd)
{
    for (int i = 0; i < MAX_FDS; ++i)
    {
        ctx->pollfds[i].fd     = -1;
        ctx->pollfds[i].events = POLLIN;
    }

    // add the server fd to the read set
    ctx->pollfds[0].fd = sfd;
}

static void add_client(int cfd, server_ctx_t* ctx)
{
    int i;
    ctx->nready--;
    for (i = 0; i < FD_SETSIZE; ++i)
    {
        if (ctx->pollfds[i].fd < 0)
        {
            // add a connected descriptor to the set
            set_nonblocking(cfd);
            ctx->pollfds[i].fd = cfd;
            break;
        }
    }

    if (i == MAX_FDS)
    {
        error_exit("too many clients");
    }

    printf("[+] Accepted new client connection\n");
}

static void process_ready_clients(server_ctx_t* ctx)
{
    ssize_t n_bytes;
    char buffer[BUFSIZE];
    for (int i = 0; (i < MAX_FDS) && (ctx->nready > 0); ++i)
    {
        struct pollfd* pfd = &ctx->pollfds[i];
        if ((pfd->fd > 0) && (pfd->revents & POLLIN))
        {
            // process IO on the ready client connection
            ctx->nready--;
            if ((n_bytes = read(pfd->fd, buffer, BUFSIZE)) > 0)
            {
                if (write(pfd->fd, buffer, n_bytes) != n_bytes)
                {
                    error_exit("write()");
                }
            }
            else
            {
                // close the connection
                close(pfd->fd);
                ctx->pollfds[i].fd = -1;
            }
        }
    }
}

int main(void)
{
    static server_ctx_t ctx;

    socklen_t addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));

    int sfd = server_socket(SERVER_PORT);
    if (-1 == sfd)
    {
        error_exit("server_socket()");
    }

    // make the listening socket nonblocking
    set_nonblocking(sfd);

    // initialize the server context
    initialize_server_ctx(&ctx, sfd);

    printf("[+] Listening on port %s\n", SERVER_PORT);

    for (;;)
    {
        // wait for events
        ctx.nready = poll(ctx.pollfds, MAX_FDS, -1);
        if (-1 == ctx.nready)
        {
            error_exit("poll()");
        }

        if (ctx.pollfds[0].revents & POLLIN)
        {
            // accept a new client connection
            int cfd = accept(sfd, (struct sockaddr*)&addr, &addrlen);
            if (-1 == cfd)
            {
                error_exit("accept()");
            }

            add_client(cfd, &ctx);
        }

        // process all clients that are ready for IO
        process_ready_clients(&ctx);
    }

    close(sfd);
    return EXIT_SUCCESS; 
}
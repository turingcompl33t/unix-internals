// select.c
//
// A concurrent echo server implemented with a select()-based event loop.
//
// Adapted from the implementation provided in CS:APP Chapter 12.2.

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

typedef struct server_ctx
{
    // The largest descriptor in the read set
    int maxfd; 

    // Set of interest descriptors
    fd_set read_set;

    // Set of ready descriptors
    fd_set ready_set;

    // Number of ready descriptors
    int nready;

    // High water mark index into fd array
    int maxi;

    // Static array of client fds
    int clientfd[FD_SETSIZE];
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
    ctx->maxi = -1;
    for (int i = 0; i < FD_SETSIZE; ++i)
    {
        ctx->clientfd[i] = -1;
    }

    // add the server fd to the read set
    ctx->maxfd = sfd;
    FD_ZERO(&ctx->read_set);
    FD_SET(sfd, &ctx->read_set);
}

static void add_client(int cfd, server_ctx_t* ctx)
{
    int i;
    ctx->nready--;
    for (i = 0; i < FD_SETSIZE; ++i)
    {
        if (ctx->clientfd[i] < 0)
        {
            // add a connected descriptor to the set
            set_nonblocking(cfd);
            ctx->clientfd[i] = cfd;
            FD_SET(cfd, &ctx->read_set);
            ctx->maxfd = max(cfd, ctx->maxfd);
            ctx->maxi = max(i, ctx->maxi);
            break;
        }
    }

    if (i == FD_SETSIZE)
    {
        error_exit("too many clients");
    }

    printf("[+] Accepted new client connection\n");
}

static void process_ready_clients(server_ctx_t* ctx)
{
    ssize_t n_bytes;
    char buffer[BUFSIZE];
    for (int i = 0; (i <= ctx->maxi) && (ctx->nready > 0); ++i)
    {
        int cfd = ctx->clientfd[i];
        if ((cfd > 0) && FD_ISSET(cfd, &ctx->ready_set))
        {
            // process IO on the ready client connection
            ctx->nready--;
            if ((n_bytes = read(cfd, buffer, BUFSIZE)) > 0)
            {
                if (write(cfd, buffer, n_bytes) != n_bytes)
                {
                    error_exit("write()");
                }
            }
            else
            {
                // close the connection
                close(cfd);
                FD_CLR(cfd, &ctx->read_set);
                ctx->clientfd[i] = -1;
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
        // re-initialize the ready set
        ctx.ready_set = ctx.read_set;

        // wait for events
        ctx.nready = select(ctx.maxfd + 1, &ctx.ready_set, NULL, NULL, NULL);
        if (-1 == ctx.nready)
        {
            error_exit("select()");
        }

        if (FD_ISSET(sfd, &ctx.ready_set))
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
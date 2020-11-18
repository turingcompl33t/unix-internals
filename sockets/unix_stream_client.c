// unix_stream_client.c
//
// A simple echo client over UNIX domain socket.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <sys/un.h>
#include <sys/socket.h>

#include "core.h"

#define SERVER_SOCKET_PATH "/tmp/us_xfr"

#define BUFSIZE 256

int main(void)
{
    struct sockaddr_un addr;

    int cfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == cfd)
    {
        error_exit("socket()");
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SERVER_SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (-1 == connect(cfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un) - 1))
    {
        error_exit("connect()");
    }

    ssize_t bytes_read;
    char buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);

    while ((bytes_read = read(STDIN_FILENO, buffer, BUFSIZE)) > 0)
    {
        if (write(cfd, buffer, bytes_read) != bytes_read)
        {
            error_exit("partial write()");
        }

        if (read(cfd, buffer, bytes_read) != bytes_read)
        {
            error_exit("partial read()");
        }

        printf("%s", buffer);
        fflush(stdout);
        memset(buffer, 0, BUFSIZE);
    }

    if (-1 == bytes_read)
    {
        error_exit("read()");
    }

    close(cfd);

    return EXIT_SUCCESS;
}
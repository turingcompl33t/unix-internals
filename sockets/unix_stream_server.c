// unix1.c
//
// A simple echo server over UNIX domain socket.

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <sys/un.h>
#include <sys/socket.h>

#include "core.h"

#define SERVER_SOCKET_PATH "/tmp/us_xfr"

#define BUFSIZE 256
#define BACKLOG 5

int setup_socket(const char* path)
{
    struct sockaddr_un addr;
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sfd)
    {
        error_exit("socket()");
    }

    if (strlen(path) > (sizeof(addr.sun_path) - 1))
    {
        error_exit("socket path too long");
    }

    if (-1 == remove(path) && errno != ENOENT)
    {
        error_exit("remove()");
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

    if (-1 == bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)))
    {
        error_exit("bind()");
    }

    if (-1 == listen(sfd, BACKLOG))
    {
        error_exit("listen()");
    }

    return sfd;
}

int main(void)
{
    int cfd;
    ssize_t bytes_read;
    char buffer[BUFSIZE];

    // Create the socket and bind it to the specified path
    int sfd = setup_socket(SERVER_SOCKET_PATH);

    printf("[+] Listening at %s\n", SERVER_SOCKET_PATH);

    // Serve clients iteratively
    for (;;)
    {
        cfd = accept(sfd, NULL, NULL);
        if (-1 == cfd)
        {
            error_exit("accept()");
        }

        while ((bytes_read = read(cfd, buffer, BUFSIZE)) > 0)
        {
            if (write(cfd, buffer, bytes_read) != bytes_read)
            {
                error_exit("partial write()");
            }
        }

        if (-1 == bytes_read)
        {
            error_exit("read()");
        }

        close(cfd);
    }

    close(sfd);

    return EXIT_SUCCESS;
}
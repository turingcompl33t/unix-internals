// linux_aio.c
//
// NOTE: run strace -T on the program to observe the blocking
// nature of io_submit() when used on a regular file without
// the O_DIRECT flag specified on the file descriptor.
//
// Build
//  gcc -Wall -Werror -std=gnu11 linux_aio.c -o linux_aio.out

#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <linux/aio_abi.h>
#include <sys/syscall.h>

#define BUFSIZE (4096)

static void fatal_error(const char* msg)
{
    fprintf(stderr, "[-] %s (%d)\n", msg, errno);
    exit(EXIT_FAILURE);
}

inline static int io_setup(unsigned nr, aio_context_t* ctx)
{
    return syscall(__NR_io_setup, nr, ctx);
}

inline static int io_destroy(aio_context_t ctx)
{
    return syscall(__NR_io_destroy, ctx);
}

inline static int io_submit(aio_context_t ctx, long nr, struct iocb** iocbpp)
{
    return syscall(__NR_io_submit, ctx, nr, iocbpp);
}

inline static int io_getevents(
    aio_context_t ctx, 
    long min_nr, 
    long max_nr, 
    struct io_event* events, 
    struct timespec* timeout)
{
    return syscall(__NR_io_getevents, ctx, min_nr, max_nr, events, timeout);
}

int main(void)
{
    const int file = open("/etc/passwd", O_RDONLY);
    if (-1 == file)
    {
        fatal_error("open()");
    }

    aio_context_t ctx = { 0 };
    int status = io_setup(128, &ctx);
    if (-1 == status)
    {
        fatal_error("io_setup()");
    }

    char buffer[BUFSIZE];
    struct iocb cb = {
        .aio_fildes     = file,
        .aio_lio_opcode = IOCB_CMD_PREAD,
        .aio_buf        = (uint64_t)&buffer[0],
        .aio_nbytes     = BUFSIZE
    };

    struct iocb* list_of_iocb[1] = {&cb};
    status = io_submit(ctx, 1, list_of_iocb);
    if (-1 == status)
    {
        fatal_error("io_submit()");
    }

    struct io_event events[1] = {};
    status = io_getevents(ctx, 1, 1, events, NULL);
    if (-1 == status)
    {
        fatal_error("io_getevents()");
    }

    printf("[+] read %lld bytes from /etc/passwd\n", events[0].res);

    io_destroy(ctx);
    close(file);

    return EXIT_SUCCESS;
}
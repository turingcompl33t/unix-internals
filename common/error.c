
// error.c
//
// Common error handling routines adapted from TLPI.

#include <stdarg.h>

#include "error.h"
#include "core.h"
#include "ename.c.inc"

#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif
static void terminate(Boolean use_exit3)
{
    char *s;
    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (use_exit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

static void output_error(
    Boolean     use_err, 
    int         err, 
    Boolean     flush_stdout,
    const char* format, 
    va_list     ap
    )
{
    #define BUF_SIZE 512

    char buf[BUF_SIZE];
    char user_msg[BUF_SIZE]; 
    char err_msg[BUF_SIZE];

    vsnprintf(user_msg, BUF_SIZE, format, ap);

    if (use_err)
        snprintf(err_msg, BUF_SIZE, " [%s %s]",
                (err > 0 && err <= MAX_ENAME) ?
                ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(err_msg, BUF_SIZE, ":");

#if __GNUC__ >= 7
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif
    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", err_msg, user_msg);
#if __GNUC__ >= 7
#pragma GCC diagnostic pop
#endif

    if (flush_stdout)
        fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);
}

void error_msg(const char *format, ...)
{
    va_list argList;
    int saved_errno;

    saved_errno = errno;

    va_start(argList, format);
    output_error(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    errno = saved_errno;
}

void error_exit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    output_error(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}
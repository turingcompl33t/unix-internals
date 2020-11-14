// error.h
//
// Common error handling routines adapted from TLPI.

#ifndef ERROR_H
#define ERROR_H

#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void error_msg(const char *format, ...);

void error_exit(const char *format, ...) NORETURN ;

#endif  // ERROR_H

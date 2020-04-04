// libvisibility.c
//
// Demo of gcc/clang visibility attribute.

#include <stdio.h>

#include "libvisibility.h"

__attribute__((visibility ("default")))
void default_fn(void)
{
    puts("default_fn()");
}

__attribute__((visibility ("hidden")))
void hidden_fn(void)
{
    puts("hidden_fn()");
}
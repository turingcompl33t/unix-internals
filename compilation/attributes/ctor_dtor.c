// ctor_dtor.c
//
// Demo of gcc/clang constructor and destructor attributes.
//
// Build
//  clang -o ctor_dtor.out -std=c11 -Wall ctor_dtor.c
//
//  gcc -o ctor_dtor.out -std=c11 -Wall ctor_dtor.c


#include <stdio.h>

__attribute__((constructor))
void ctor()
{
    printf("constructor called\n");
}

__attribute__((destructor))
void dtor()
{
    printf("destructor called\n");
}

int main()
{
    printf("main called\n");
    return 0;
}

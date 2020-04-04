// visibility.c
//
// Demo of gcc/clang visibility attribute.
//
// Build: Mac OSX
//  clang -shared -fPIC -o libvisibility.dylib libvisibility.c
//  clang -o visibility.out visibility.c libvisibility.dylib

#include "libvisibility.h"

int main(void)
{
    default_fn();

    // linker error
    // hidden_fn();  
}
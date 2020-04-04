// main.c
//
// Build: Mac OSX 
//  clang -shared -fPIC x.c y.c -o libxy.dylib
//  clang -o main.out main.c libxy.dylib
//
//  gcc -shared -fPIC x.c y.c -o libxy.dylib
//  gcc -o main.out main.c libxy.dylib
//

#include "x.h"
#include "y.h"

int main(void)
{
    print_x();
    print_y();
}
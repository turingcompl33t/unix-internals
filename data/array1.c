// array1.c
//
// Variable-sized arrays.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_at_index(size_t dim, int arr[dim][dim], size_t i, size_t j) {
    return arr[i][j];
}

int main(void) {
    const size_t dim = 4;
    int arr[dim][dim];
    for (size_t i = 0; i < dim; ++i) {
        for (size_t j = 0; j < dim; ++j) {
            arr[i][j] = i*dim + j;
        }
    }

    const int x = get_at_index(dim, arr, 1, 0);
    printf("[+] Read %d\n", x);

    return EXIT_SUCCESS;
}
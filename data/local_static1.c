// local_static1.c
//
// Basic semantics of local static variables.

#include <stdio.h>
#include <stdlib.h>

void do_work(size_t invocation)
{
    static size_t local = 0;
    printf("[+] Invovation number %zu; static local value = %zu\n", invocation, local++);
}

int main(void)
{
	for (size_t i = 0; i < 5; ++i) 
	{
		do_work(i);
	}

	return EXIT_SUCCESS;
}
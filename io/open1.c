// open1.c
//
// File descriptor numbers assigned for a given process.

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "core.h"

int main(void) 
{
  int fd1 = open("open1.c", O_RDONLY);
  if (-1 == fd1) 
  {
    error_exit("open()");
  }

  printf("fd1 = %d\n", fd1);
  close(fd1);

  int fd2 = open("open1.c", O_RDONLY);
  if (-1 == fd2) 
  {
    error_exit("open()");
  }

  printf("fd2 = %d\n", fd2);
  close(fd2);

  return EXIT_SUCCESS;
}
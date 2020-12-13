// open2.c
//
// Manually closing automatically-opened file descriptors.

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "core.h"

int main(void) 
{
  close(STDIN_FILENO);
  close(STDERR_FILENO);

  int fd1 = open("open2.c", O_RDONLY);
  if (-1 == fd1)
  {
    error_exit("open()");
  }

  fprintf(stdout, "fd1 = %d\n", fd1);

  int fd2 = open("open2.c", O_RDONLY);
  if (-1 == fd2)
  {
    error_exit("open()");
  }

  fprintf(stdout, "fd2 = %d\n", fd2);

  close(fd1);
  close(fd2);

  return EXIT_SUCCESS;
}
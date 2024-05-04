#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "webserver.h"

int main(int argc, char *argv[])
{
  printf("pid: %d, ppid: %d \n", getpid(), getppid());
  if (webserver() != 0)
  {
    perror("webserver");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
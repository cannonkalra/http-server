#include <stdio.h>
#include <stdlib.h>
#include "webserver.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
  if (webserver(PORT, BUFFER_SIZE) != 0)
  {
    perror("webserver");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
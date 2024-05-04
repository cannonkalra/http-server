#include "webserver.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

int webserver(int port)
{
  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    perror("webserver (socket)");
    return EXIT_FAILURE;
  }
  printf("socket created successfully\n");

  // Create the address to bind the socket to
  struct sockaddr_in host_addr;
  int host_addrlen = sizeof(host_addr);

  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(port);
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind the socket to the address
  if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0)
  {
    perror("webserver (bind)");
    return EXIT_FAILURE;
  }
  printf("socket successfully bound to address\n");

  // Listen for incoming connections
  if (listen(sockfd, SOMAXCONN) != 0)
  {
    perror("webserver (listen)");
    return 1;
  }
  printf("server listening for connections\n");

  for (;;)
  {
    // Accept incoming connections
    int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr,
                           (socklen_t *)&host_addrlen);
    if (newsockfd < 0)
    {
      perror("webserver (accept)");
      continue;
    }
    printf("connection accepted\n");

    close(newsockfd);
  }

  return EXIT_SUCCESS;
}
#include "webserver.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "request.h"
#include <stdbool.h>

int webserver(int port, int buffer_size)
{
  char buffer[buffer_size];
  char resp[] = "HTTP/1.0 200 OK\r\n"
                "Server: webserver-c\r\n"
                "Content-type: text/html\r\n\r\n"
                "<html>hello, world</html>\r\n";

  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    perror("webserver (socket)");
    return EXIT_FAILURE;
  }
  printf("socket created successfully socket_fd: %d \n", sockfd);

  // Create the address to bind the socket to
  struct sockaddr_in host_addr;
  int host_addrlen = sizeof(host_addr);

  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(port);
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Create client address
  struct sockaddr_in client_addr;
  int client_addrlen = sizeof(client_addr);

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
    return EXIT_FAILURE;
  }
  printf("server listening for connections on sockfd: %d \n\n", sockfd);

  while (true)
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

    // Get client address
    int sockn = getsockname(newsockfd, (struct sockaddr *)&client_addr,
                            (socklen_t *)&client_addrlen);
    if (sockn < 0)
    {
      perror("webserver (getsockname)");
      continue;
    }

    printf("handling request with sockfd: %d \n", newsockfd);

    pthread_t thread;
    // pthread_create(thread, NULL, request_handle, (void *)newsockfd);
    // request_handle(newsockfd);

    if (pthread_create(&thread, NULL, request_handle, (void *)newsockfd) != 0)
    {
      perror("pthread_create");
      return EXIT_FAILURE;
    }
    // // Read from the socket
    // int valread = read(newsockfd, buffer, buffer_size);
    // if (valread < 0)
    // {
    //   perror("webserver (read)");
    //   continue;
    // }

    // // Read the request
    // char method[buffer_size], uri[buffer_size], version[buffer_size];
    // sscanf(buffer, "%s %s %s", method, uri, version);
    // printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr),
    //        ntohs(client_addr.sin_port), method, version, uri);

    // // Write to the socket
    // int valwrite = write(newsockfd, resp, strlen(resp));
    // if (valwrite < 0)
    // {
    //   perror("webserver (write)");
    //   continue;
    // }
    // printf("closing connection with sockfd: %d \n\n", newsockfd);
    // close(newsockfd);
  }
  return EXIT_SUCCESS;
}
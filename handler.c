#include "handler.h"

#define BUFFER_SIZE 1024
#define SLEEP_MS 1200

void handle_client(int client_fd)
{
  char buffer[BUFFER_SIZE];
  char resp[] = "HTTP/1.0 200 OK\r\n"
                "Server: webserver-c\r\n"
                "Content-type: text/html\r\n\r\n"
                "<html>hello, world</html>\r\n";

  // Read from the socket
  int valread = read(client_fd, buffer, sizeof(buffer));
  if (valread < 0)
  {
    perror("webserver (read)");
  }

  usleep(SLEEP_MS * 1000);
  // Write to the socket
  int valwrite = write(client_fd, resp, strlen(resp));
  if (valwrite < 0)
  {
    perror("webserver (write)");
  }
  // Close the client socket when done
  close(client_fd);
}

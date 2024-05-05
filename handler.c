#include <sys/mman.h>
#include "handler.h"
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define MAX_BUFFER 8192

#define SLEEP_MS 8

void multi_thread_handler(void *arg)
{
  int client_fd = *((int *)arg);
  handle_client(client_fd);
}

void handle_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
  char buf[BUFFER_SIZE], body[BUFFER_SIZE];

  // Create the body of error message first (have to know its length for header)
  sprintf(body, ""
                "<!doctype html>\r\n"
                "<head>\r\n"
                "  <title>OSTEP WebServer Error</title>\r\n"
                "</head>\r\n"
                "<body>\r\n"
                "  <h2>%s: %s</h2>\r\n"
                "  <p>%s: %s</p>\r\n"
                "</body>\r\n"
                "</html>\r\n",
          errnum, shortmsg, longmsg, cause);

  // Write out the header information for this response
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  write(fd, buf, strlen(buf));

  sprintf(buf, "Content-Type: text/html\r\n");
  write(fd, buf, strlen(buf));

  sprintf(buf, "Content-Length: %lu\r\n\r\n", strlen(body));
  write(fd, buf, strlen(buf));

  // Write out the body last
  write(fd, body, strlen(body));
}

void handle_client(int client_fd)
{
  printf("handling connection client_fd: %d, client thread id: %d \n", client_fd, (int)(intptr_t)pthread_self());
  char read_buffer[BUFFER_SIZE], buffer[BUFFER_SIZE], method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE], filename[BUFFER_SIZE];
  int file_fd;
  char *file_buffer;

  // Read from the socket
  ssize_t valread = read(client_fd, read_buffer, sizeof(read_buffer));
  if (valread < 0)
  {
    perror("webserver (read)");
    exit(EXIT_FAILURE);
  }
  sscanf(read_buffer, "%s %s %s", method, uri, version);
  printf("method:%s uri:%s version:%s\n", method, uri, version);

  if (strcasecmp(method, "GET"))
  {
    handle_error(client_fd, method, "501", "Not Implemented", "server does not implement this method");
    perror("method not implemented \n");
    close(client_fd);
    return;
  }

  // make filename to index.html if '/' else filename
  strcpy(filename, strcmp(uri, "/") == 0 ? "index.html" : uri);

  if (strcmp(filename, "index.html") != 0)
  {
    handle_error(client_fd, filename, "404", "Not found", "server could not find this file");
    perror("index.html");
    close(client_fd);
    return;
  }

  usleep(SLEEP_MS * 1000);

  file_fd = open(filename, O_RDONLY, 0);

  if (file_fd < 0)
  {
    perror("file open error");
  }
  // rather than call read() to read the file into memory,
  // which would require that we allocate a buffer, we memory-map the file
  file_buffer = mmap(0, MAX_BUFFER, PROT_READ, MAP_PRIVATE, file_fd, 0);

  // close the file after reading
  close(file_fd);

  printf("file size: %lu \n", strlen(file_buffer));

  // put together response

  sprintf(buffer, ""
                  "HTTP/1.0 200 OK\r\n"
                  "Server: custom-http\r\n"
                  "Content-Length: %d\r\n"
                  "Content-Type: %s\r\n\r\n",
          (int)strlen(file_buffer), "text/html");
  write(client_fd, buffer, strlen(buffer));

  // Write to the socket
  int valwrite = write(client_fd, file_buffer, strlen(file_buffer));
  if (valwrite < 0)
  {
    perror("webserver (write)");
  }

  munmap(file_buffer, strlen(file_buffer));

  printf("closing_ connection client_fd: %d, client thread id: %d \n", client_fd, (int)(intptr_t)pthread_self());
  // Close the client socket when done
  close(client_fd);
  return;
}

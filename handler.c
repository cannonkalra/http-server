#include <sys/mman.h>
#include "handler.h"
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define MAX_BUFFER 8192

#define SLEEP_MS 1200

void handle_client(int client_fd)
{
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
    // request_error(client_fd, method, "501", "Not Implemented", "server does not implement this method");
    perror("method not implemented \n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(uri, "/") == 0)
  {
    strcpy(filename, "index.html");
  }

  // TODO: handle error as 403, 404
  if (strcmp(filename, "") == 0)
  {
    perror("index.html");
    exit(EXIT_FAILURE);
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
  // Close the client socket when done
  close(client_fd);
}

void *handle_client_multi_thread(void *arg)
{
  int client_fd = *((int *)arg);
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
    // request_error(client_fd, method, "501", "Not Implemented", "server does not implement this method");
    perror("method not implemented \n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(uri, "/") == 0)
  {
    strcpy(filename, "index.html");
  }

  // TODO: handle error as 403, 404
  if (strcmp(filename, "") == 0)
  {
    perror("index.html");
    exit(EXIT_FAILURE);
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
  // Close the client socket when done
  close(client_fd);
}

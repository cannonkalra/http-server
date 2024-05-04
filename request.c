#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "request.h"
#include "io_helper.h"

#define MAXBUF (8192)

#define SLEEP_MS 2200

int request_parse_uri(char *uri, char *filename, char *cgiargs)
{
  char *ptr;

  if (!strstr(uri, "cgi"))
  {
    // static
    strcpy(cgiargs, "");
    sprintf(filename, ".%s", uri);
    if (uri[strlen(uri) - 1] == '/')
    {
      strcat(filename, "index.html");
    }
    return 1;
  }
}

// fills in the filetype given the filename
void request_get_filetype(char *filename, char *filetype)
{
  if (strstr(filename, ".html"))
    strcpy(filetype, "text/html");
  else if (strstr(filename, ".gif"))
    strcpy(filetype, "image/gif");
  else if (strstr(filename, ".jpg"))
    strcpy(filetype, "image/jpeg");
  else
    strcpy(filetype, "text/plain");
}

void request_serve_static(int fd, char *filename, int filesize)
{
  int srcfd;
  char *srcp, filetype[MAXBUF], buf[MAXBUF];

  request_get_filetype(filename, filetype);
  // simulate a i/o wait
  usleep(SLEEP_MS * 1000);

  srcfd = open(filename, O_RDONLY, 0);

  // rather than call read() to read the file into memory,
  // which would require that we allocate a buffer, we memory-map the file
  srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
  close(srcfd);

  // put together response
  sprintf(buf, ""
               "HTTP/1.0 200 OK\r\n"
               "Server: Custom WebServer\r\n"
               "Content-Length: %d\r\n"
               "Content-Type: %s\r\n\r\n",
          filesize, filetype);

  write(fd, buf, strlen(buf));

  //  Writes out to the client socket the memory-mapped file
  write(fd, srcp, filesize);
  munmap(srcp, filesize);
}

void request_error(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
  char buf[MAXBUF], body[MAXBUF];

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

void request_handle(int fd)
{
  struct stat sbuf;
  char buf[MAXBUF], method[MAXBUF], uri[MAXBUF], version[MAXBUF];
  char filename[MAXBUF], cgiargs[MAXBUF];

  readline(fd, buf, MAXBUF);
  sscanf(buf, "%s %s %s", method, uri, version);
  printf("method:%s uri:%s version:%s\n", method, uri, version);

  if (strcasecmp(method, "GET"))
  {
    request_error(fd, method, "501", "Not Implemented", "server does not implement this method");
    return;
  }

  printf("main thread id: %d\n", pthread_self());
  // request_read_headers(fd);
  if (
      strcmp(filename, "./") == 0 || strlen(filename) == 0)
  {
    strcpy(filename, "index.html");
  }

  if (stat(filename, &sbuf) < 0)
  {
    request_error(fd, filename, "404", "Not found", "server could not find this file");
    return;
  }

  if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
  {
    request_error(fd, filename, "403", "Forbidden", "server could not read this file");
    return;
  }
  request_serve_static(fd, filename, sbuf.st_size);
}

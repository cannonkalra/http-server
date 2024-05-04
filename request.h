#ifndef __REQUEST_H__

void request_handle(int fd);

ssize_t readline(int fd, void *buf, size_t maxlen)
{
  char c;
  char *bufp = buf;
  int n;
  for (n = 0; n < maxlen - 1; n++)
  { // leave room at end for '\0'
    int rc;
    if ((rc = read(fd, &c, 1)) == 1)
    {
      *bufp++ = c;
      if (c == '\n')
        break;
    }
    else if (rc == 0)
    {
      if (n == 1)
        return 0; /* EOF, no data read */
      else
        break; /* EOF, some data was read */
    }
    else
      return -1; /* error */
  }
  *bufp = '\0';
  return n;
}

#endif // __REQUEST_H__
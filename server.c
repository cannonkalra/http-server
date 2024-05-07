#include "handler.h"
#include "pool.h"
#include <fcntl.h>
#define PORT 8080
#define MULTI_THREAD 1

#define THREADS 8
#define ITEMS 100

// Function to handle communication with the client

int main()
{
  pid_t pid = getpid();
  pid_t ppid = getppid();

  printf("pid: %d, ppid: %d \n", pid, ppid);
  printf("pagesize: %dkB \n", getpagesize() / 1024);
  // Create a socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct rlimit rlim;
  printf("max heap size: %llu bytes\n", rlim.rlim_cur);

  int option = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  if (server_fd < 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  printf("socket created successfully serverfd: %d\n", server_fd);

  // Create the address to bind the socket to
  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind the socket to the address
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("webserver (bind)");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  printf("socket successfully bound to address\n");

  if (listen(server_fd, SOMAXCONN) < 0)
  {
    perror("webserver (listen)");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  printf("server listening for connections\n");
  printf("main thread id: %d\n\n", (int)(intptr_t)pthread_self());

  tpool_t *tm;

  tm = tpool_create(THREADS);

  struct rusage usage;

  // Loop to accept and handle incoming connections sequentially
  while (true)
  {
    printf("threadpool working threads count:%lu \n", tm->working_cnt);
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int *client_fd = (int *)malloc(sizeof(int));
    *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (*client_fd < 0)
    {
      perror("webserver (accept)");
      // Continue to the next loop iteration if accept fails
      continue;
    }
    printf("connection (accepted)\n");

    if (!MULTI_THREAD)
    {
      handle_client(*client_fd);
    }
    else
    {
      tpool_add_work(tm, multi_thread_handler, client_fd);
    }
    // Handle the client in the same thread (sequentially)
    getrusage(RUSAGE_SELF, &usage);
    printf("memusage: %ld kB \n", usage.ru_maxrss / 1024);
  }

  // Close the server socket before exiting
  close(server_fd);

  return 0;
}
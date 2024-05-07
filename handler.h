#ifndef __HANDLER__
#define __HANDLER__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

void handle_client(int);
void multi_thread_handler(void *);

#endif // __HANDLER__

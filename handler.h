#ifndef __HANDLER__
#define __HANDLER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

void handle_client(int);
void *handle_client_multi_thread(void *);

#endif // __HANDLER__

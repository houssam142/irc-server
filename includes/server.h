#pragma once

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/epoll.h>
# include <stdbool.h>
# include <errno.h>
# include <fcntl.h>
# include "parse.h"

#define MAX_EVENTS 1000
# define MAX_CLIENTS 1024

extern int max_file_descriptors;

typedef struct epoll_event epoll_t;
typedef struct s_client t_client;

typedef struct server
{
  int port;
  char *server_name;
  char *host;
  char *password;
} t_server;


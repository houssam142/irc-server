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

typedef enum section
{
  SERVER,
  NETWORK,
  OPERATOR,
  LINK,
  NONE
} t_section;

typedef enum e_state
{
  SINGLE_TABLE,
  ARRAY_TABLE,
  KEY_VALUE,
  EMPTY,
  INVALID
} t_state;

typedef struct server
{
  char **buff;
  char *server_name;
  char *host;
  char *password;
  int port;
  int link_count;
  bool server_sect_found;
} t_server;


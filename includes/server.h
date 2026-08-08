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
# include <signal.h>
# include <arpa/inet.h>

#define MAX_EVENTS 1000
# define MAX_CLIENTS 1024

# define MAX_PASSWORD_LENGTH 64

extern int max_file_descriptors;

typedef struct epoll_event epoll_t;
typedef struct s_client t_client;
typedef struct s_link t_link;

typedef enum section
{
  SERVER,
  NETWORK,
  OPERATOR,
  LINK,
  NONE
} t_section;

enum server_state
{
  SERVER_DISCONNECTED,
  SERVER_CONNECTED,
  SERVER_AUTHENTIFICATED
};

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
  t_link *links;
  char **buff;
  char *server_name;
  char *hostname;
  char *host;
  char password[MAX_PASSWORD_LENGTH + 1];
  int port;
  int link_count;
  bool server_sect_found;
  server_state serv_state;
} t_server;


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

#define MAX_EVENTS 2000

# define INITIAL_CLIENT_CAPACITY 8

# define MAX_PASSWORD_LENGTH 64

typedef struct epoll_event epoll_t;
typedef struct s_client t_client;
typedef struct s_link t_link;

typedef struct s_remote_user
{
  char name[64];
  char username[64];
  t_link *server_origin;
} t_remote_user;

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
  t_remote_user *remote_users;
  t_client *cls;
  char **buff;
  char *server_name;
  char *hostname;
  char *host;
  char buff_recv[2048];
  char password[MAX_PASSWORD_LENGTH + 1];
  int port;
  int link_count;
  size_t client_count;
  size_t client_cap;
  size_t client_id;
  bool server_sect_found;
  enum server_state serv_state;
} t_server;

void  free_client(t_server *server, int client_fd);

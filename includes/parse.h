#pragma once

# include "server.h"
# include "types.h"
# include "../libft/libft.h"
# include <ctype.h>
# include "get_next_line.h"


typedef enum e_conn_type t_connection_type;

typedef struct server t_server;

typedef enum e_state t_state;

typedef enum section t_section;

typedef struct s_link
{
  char *name;
  char *password;
  int port;
  int fd;
} t_link;

typedef struct s_client
{
  t_server *server_side;
  char *nick;
  char *username;
  char *hostname;
  char *real_name;
  char *error_message;
  char *server_password;
  int   fd;
  t_connection_type conn_type;
  bool  is_matched;
} t_client;

int parse_input(char *buff, t_server *server, int client_fd);
int get_array_size(char **arr);
int parse_config(char *file, t_server *server);
bool is_key_allowed(char *key, t_section sect);


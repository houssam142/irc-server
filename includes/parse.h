#pragma once

# include "server.h"
# include "../libft/libft.h"
# include "get_next_line.h"

typedef struct server t_server;

typedef enum e_state t_state;

typedef enum section t_section;

typedef struct s_client
{
  t_server *server;
  char *nick;
  char *username;
  char *hostname;
  char *real_name;
  char *error_message;
  char *server_password;
  int   fd;
  bool  is_matched;
} t_client;

int parse_input(char *buff, t_client cls[], int client_fd);
int get_array_size(char **arr);
void  free_client(t_client cls[], int client_fd);
int parse_config(char *file, t_server *server);
bool is_key_allowed(char *key, t_section sect);


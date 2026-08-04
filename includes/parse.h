#pragma once

# include "server.h"
# include "../libft/libft.h"
# include "get_next_line.h"

typedef struct server t_server;

typedef enum e_state t_state;

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
int is_server(char *sect, t_state *state);
int is_operator(char *sect, t_state *state);
int is_link(char *sect, t_state *state);
int is_network(char *sect, t_state *state);



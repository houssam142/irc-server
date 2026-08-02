#pragma once

# include "server.h"
# include "../libft/libft.h"

typedef struct s_client
{
  char *nick;
  char *username;
  char *hostname;
  char *real_name;
  int   fd;
  char *error_message;
} t_client;

bool parse_input(char *buff, t_client cls[], int client_fd);
int get_array_size(char **arr);

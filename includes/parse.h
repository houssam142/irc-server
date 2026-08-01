#pragma once

# include "server.h"
# include "../libft/libft.h"

typedef struct s_client
{
  char *nick;
  char *username;
  char *hostname;
  char *real_name;
  struct s_client *next;
} t_client;

bool parse_input(char *buff, t_client *cls);
int get_array_size(char **arr);

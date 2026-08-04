#include "../includes/parse.h"

int is_operator(char *sect, t_state *state)
{
  if (!strcmp(sect, "[[operator]]"))
  {
    *state = OPERATOR;
    return 0;
  }
  return 1;
}

int is_link(char *sect, t_state *state)
{
  if (!strcmp(sect, "[[link]]"))
  {
    *state = LINK;
    return 0;
  }
  return 1;
}

int is_network(char *sect, t_state *state)
{
  if (!strcmp(sect, "[network]"))
  {
    *state = NETWORK;
    return 0;
  }
  return 1;
}

int is_server(char *sect, t_state *state)
{
  if (!strcmp(sect, "[server]"))
  {
    *state = SERVER;
    return 0;
  }
  return 1;
}

int get_array_size(char **arr)
{
  int i;

  i = 0;
  while (arr[i] != NULL)
    i++;
  return (i);
}

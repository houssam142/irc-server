#include "../includes/parse.h"

char *parse_line_by_line(char *line)
{
  int i;
  char *copy;

  i = 0;
  copy = malloc(ft_strlen(line) + 1);
  if (!copy)
  {
    perror("malloc");
    return NULL;
  }
  while (i < (int)ft_strlen(line))
  {
    if (line[i] == '#')
    {
      while (i < (int)ft_strlen(line))
        i++;
      continue;
    }
    copy[i] = line[i];
    i++;
  }
  copy[i] = '\0';
  return copy;
}

int check_brackets(char *line)
{
  
}

int validate_config(t_server *server)
{
  int i;
  char *tmp = NULL;
  t_state state;

  i = 0;
  state = NONE;
  while (server->buff[i] != NULL)
  {
    tmp = ft_strtrim(server->buff[i], " \t\n");
    if (!tmp)
      return -1;
    if (tmp[0] == '[')
    {
      if (!strncmp(tmp, "[[", 2))
      if (is_server(tmp, &state) && is_network(tmp, &state) && is_link(tmp, &state)
          && is_operator(tmp, &state))
      {
        fprintf(stderr, "section %s not supported\n", tmp);
        return 1;
      }
    }
    free(tmp);
    i++;
  }
  return 0;
}

int parse_config(char *file, t_server *server)
{
  char *line = NULL;
  int line_count = 0;
  int i = 0;
  char **arr = NULL;
  char *tmp2 = NULL;
  int file_fd = open(file, O_RDONLY, 0400);
  memset(server, 0, sizeof(t_server));
  server->server_sect_found = false;
  if (file_fd < 0)
  {
    perror("open");
    return -1;
  }
  while ((line = get_next_line(file_fd)) != NULL)
  {
    line_count++;
    free(line);
  }
  lseek(file_fd, 0, SEEK_SET);
  arr = malloc(sizeof(char *) * (line_count + 1));
  if (!arr)
  {
    perror("malloc");
    return -1;
  }
  while ((line = get_next_line(file_fd)))
  {
    char *tmp = ft_strtrim(line, " \n\t");

    if (!tmp[0])
    {
      free(tmp);
      continue;
    }
    tmp2 = parse_line_by_line(tmp);
    if (!tmp2[0])
    {
      free(tmp);
      continue;
    }
    arr[i] = tmp2;
    free(line);
    free(tmp);
    i++;
  }
  arr[i] = NULL;
  server->buff = arr;
  if (validate_config(server))
    return 1;
  for (int i = 0; server->buff[i]; i++)
    printf("%s\n", server->buff[i]);
  close(file_fd);
  ft_free(arr);
  exit(1);
  return 0;
}

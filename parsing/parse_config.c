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

t_state classify_line(char *tmp, t_state state)
{
  char **key_value = ft_split(tmp, '=');
  if (!key_value)
    return -1;
  if (!tmp[0])
    state = EMPTY;
  else if (tmp[0] == '[' && tmp[ft_strlen(tmp) - 1] == ']'
      && ft_strncmp(tmp, "[[", 2) && ft_strncmp(&tmp[ft_strlen(tmp) - 2], "]]", 2))
    state = SINGLE_TABLE;
  else if (!ft_strncmp(tmp, "[[", 2) && !ft_strncmp(&tmp[ft_strlen(tmp) - 2], "]]", 2))
    state = ARRAY_TABLE;
  else if (get_array_size(key_value) == 2)
    state = KEY_VALUE;
  else
    state = INVALID;
  ft_free(key_value);
  return state;
}

int check_value_syntax(char *key, char *value)
{
  if (!strcmp(key, "port") || !strcmp(key, "max_clients") || !strcmp(key, "max_channels_per_user")
      || !strcmp(key, "max_nickname_length") || !strcmp(key, "ping_interval") || !strcmp(key, "timeout")
      || !strcmp(key, "retry_interval"))
  {
    for (int i = 0; i < ft_strlen(value); i++)
    {
      if (!ft_isdigit(value[i]))
      {
        fprintf(stderr, "Syntax Error: Expected '%s' to have a value of digits but instead '%s'.\n", key, value);
        return 1;
      }
    }
  }
  if (!strcmp(key, ""))
  return 0;
}

int validate_config(t_server *server)
{
  int i;
  char *tmp = NULL;
  
  t_state state;
  t_section curr_sect;

  i = 0;
  state = INVALID;
  curr_sect = NONE;
  while (server->buff[i] != NULL)
  {
    tmp = ft_strtrim(server->buff[i], " \t\n");
    if (!tmp)
      return -1;
    state = classify_line(tmp, state);
    if (state == SINGLE_TABLE)
    {
      if (!strcmp(tmp, "[server]"))
        curr_sect = SERVER;
      else
      {
        free(tmp);
        fprintf(stderr, "Syntax Error: Invalid section header.\nExpected [server].\n");
        return 1;
      }
    }
    else if (state == ARRAY_TABLE)
    {
      if (!strcmp(tmp, "[[link]]"))
        curr_sect = LINK;
      else
      {
        free(tmp);
        fprintf(stderr, "Syntax Error: Invalid section header.\nExpected [[link]].\n");
        return 1;
      }
    }
    else if (state == KEY_VALUE)
    {
      if (curr_sect == NONE)
      {
        free(tmp);
        fprintf(stderr, "Syntax Error: key-value pairs should be under a section header.\nExpected [server], [network], [[link]] or [[operator]].\n");
        return 1;
      }
      if (curr_sect == SERVER)
      {
        char **key_value = ft_split(tmp, '=');
        if (!key_value)
          return -1;
        char *key = ft_strtrim(key_value[0], " \t");
        char *value = ft_strtrim(key_value[1], " \t");
        if (!is_key_allowed(key, curr_sect))
        {
          fprintf(stderr, "Invalid key '%s' in section 'server'.\nAllowed keys are: name, password and host\n", key);
          return 1;
        }
        if (check_value_syntax(key, value))
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

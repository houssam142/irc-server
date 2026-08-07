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

int check_server_eles_syntax(char *key, char *value, t_server *server)
{
  if (!strcmp(key, "port"))
  {
    for (size_t i = 0; i < ft_strlen(value); i++)
    {
      if (!ft_isdigit(value[i]))
      {
        fprintf(stderr, "Syntax Error: Unexpected character %c in port's value.\n", value[i]);
        return 1;
      }
    }
    if (ft_strlen(value) > 5)
    {
      fprintf(stderr, "Port value should be between 1 and 65535.\n");
      return 1;
    }
    server->port = atoi(value);
    if (server->port < 1 || server->port > 65535)
    {
      fprintf(stderr, "Port value should be between 1 and 65535.\n");
      return 1;
    }
  }
  else if (!strcmp(key, "name"))
  {
    if (value[0] == '\"' && value[ft_strlen(value) - 1] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing closing double quote in %s.\n", value);
      return 1;
    }
    else if (value[ft_strlen(value) - 1] == '\"' && value[0] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing opening double quote in %s.\n", value);
      return 1;
    }
    else if (value[0] != '\"' && value[ft_strlen(value) - 1] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing double quotes in %s.\n", value);
      return 1;
    }
    for (size_t i = 0; i < ft_strlen(value); i++)
    {
      if (!ft_isalpha(value[i]) && value[i] != '-' && value[i] != '\"')
      {
        fprintf(stderr, "Syntax Error: Expected '%s' to have a string value but instead '%s'.\n", key, value);
        return 1;
      }
    }
    char *tmp = ft_strtrim(value, "\"");
    server->server_name = ft_strdup(tmp);
    free(tmp);
  }
  else if (!strcmp(key, "password"))
  {
    if (value[0] == '\"' && value[ft_strlen(value) - 1] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing closing double quote in %s.", value);
      return 1;
    }
    else if (value[ft_strlen(value) - 1] == '\"' && value[0] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing opening double quote in %s.", value);
      return 1;
    }
    else if (value[0] != '\"' && value[ft_strlen(value) - 1] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing double quotes in %s.", value);
      return 1;
    }
    char *tmp = ft_strtrim(value, "\"");
    server->password = ft_strdup(tmp);
    free(tmp);
  }
  return 0;
}

int check_link_eles_syntax(char *key, char *value)
{
  if (!strcmp(key, "port"))
  {
    for (size_t i = 0; i < ft_strlen(value); i++)
    {
      if (!ft_isdigit(value[i]))
      {
        fprintf(stderr, "Syntax Error: Unexpected character %c in port's value.\n", value[i]);
        return 1;
      }
    }
    if (ft_strlen(value) > 5)
    {
      fprintf(stderr, "Port value should be between 1 and 65535.\n");
      return 1;
    }
    int port = atoi(value);
    if (port < 1 ||port > 65535)
    {
      fprintf(stderr, "Port value should be between 1 and 65535.\n");
      return 1;
    }
  }
  else if (!strcmp(key, "name"))
  {
    if (value[0] == '\"' && value[ft_strlen(value) - 1] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing closing double quote in %s.\n", value);
      return 1;
    }
    else if (value[ft_strlen(value) - 1] == '\"' && value[0] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing opening double quote in %s.\n", value);
      return 1;
    }
    else if (value[0] != '\"' && value[ft_strlen(value) - 1] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing double quotes in %s.\n", value);
      return 1;
    }
    for (size_t i = 0; i < ft_strlen(value); i++)
    {
      if (!ft_isalpha(value[i]) && value[i] != '-' && value[i] != '\"')
      {
        fprintf(stderr, "Syntax Error: Expected '%s' to have a string value but instead '%s'.\n", key, value);
        return 1;
      }
    }
    char *tmp = ft_strtrim(value, "\"");
    free(tmp);
  }
  else if (!strcmp(key, "password"))
  {
    if (value[0] == '\"' && value[ft_strlen(value) - 1] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing closing double quote in %s.", value);
      return 1;
    }
    else if (value[ft_strlen(value) - 1] == '\"' && value[0] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing opening double quote in %s.", value);
      return 1;
    }
    else if (value[0] != '\"' && value[ft_strlen(value) - 1] != '\"')
    {
      fprintf(stderr, "Syntax Error: missing double quotes in %s.", value);
      return 1;
    }
  }
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
      if (server->server_sect_found != false)
      {
        free(tmp);
        fprintf(stderr, "Syntax Error: Duplicate section '[server]'\n");
        return 1;
      }
      else if (!strcmp(tmp, "[server]"))
      {
        curr_sect = SERVER;
        server->server_sect_found = true;
      }
      else
      {
        free(tmp);
        fprintf(stderr, "Syntax Error: Invalid section header.\nExpected [server].\n");
        return 1;
      }
    }
    else if (state == ARRAY_TABLE)
    {
      if (!server->server_sect_found)
      {
        free(tmp);
        fprintf(stderr, "Syntax Error: [server] section header should be the first section in the configuration file.\n");
        return 1;
      }
      if (!strcmp(tmp, "[[link]]"))
      {
        curr_sect = LINK;
        server->link_count++;
      }
      else
      {
        free(tmp);
        fprintf(stderr, "Syntax Error: Invalid section header.\nExpected [[link]] section header.\n");
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
      char **key_value = ft_split(tmp, '=');
      if (!key_value)
          return -1;
      char *key = ft_strtrim(key_value[0], " \t");
      char *value = ft_strtrim(key_value[1], " \t");
      if (!key || !value)
        return -1;
      if (curr_sect == SERVER)
      {
        if (!is_key_allowed(key, curr_sect))
        {
          ft_free(key_value);
          free(key);
          free(value);
          fprintf(stderr, "Invalid key '%s' in section [server].\nAllowed keys are: name, password and port\n", key);
          return 1;
        }
        if (check_server_eles_syntax(key, value, server))
        {
          ft_free(key_value);
          free(key);
          free(value);
          return 1;
        }
      }
      else if (curr_sect == LINK)
      {
        if (!is_key_allowed(key, curr_sect))
        {
          ft_free(key_value);
          free(key);
          free(value);
          fprintf(stderr, "Invalid key '%s' in section [[link]].\nAllowed keys are: name, password and port\n", key);
          return 1;
        }
        if (check_link_eles_syntax(key, value))
        {
          ft_free(key_value);
          free(key);
          free(value);
          return 1;
        }
      }
      ft_free(key_value);
      free(key);
      free(value);
    }
    free(tmp);
    i++;
  }
  return 0;
}

int link_key_value(t_server *server, t_link *links, int i, int curr_link)
{
  if (links[curr_link].name && links[curr_link].password
      && links[curr_link].name[0] && links[curr_link].port && links[curr_link].password[0])
    return 0;
  char **key_value = ft_split(server->buff[i], '=');
  if (!key_value)
    return -1;
  char *key = ft_strtrim(key_value[0], " \t\n");
  char *value = ft_strtrim(key_value[1], " \t\n");
  if (!key || !value)
    return -1;
  if (!strcmp(key, "name"))
    links[curr_link].name = ft_strdup(value);
  else if (!strcmp(key, "port"))
    links[curr_link].port = atoi(value);
  else if (!strcmp(key, "password"))
    links[curr_link].password = ft_strdup(value);
  return 0;
}

int store_link_values(t_server *server)
{
  int i;
  t_state state;
  t_section curr_sect;
  int curr_link;
  t_link *links = malloc(server->link_count * sizeof(t_link));
  if (!links)
  {
    perror("malloc");
    return -1;
  }
  i = -1;
  curr_link = 0;
  curr_sect = NONE;
  memset(links, 0, sizeof(t_link) * server->link_count);
  while (server->buff[++i])
  {
    state = classify_line(server->buff[i], state);
    if (state == ARRAY_TABLE)
    {
      if (!strcmp(server->buff[i], "[[link]]"))
        curr_sect = LINK;
      if (links[curr_link].name && links[curr_link].password
          && links[curr_link].name[0] &&links[curr_link].password[0] && links[curr_link].port)
        curr_link++;
    }
    else if (state == KEY_VALUE)
    {
      if (curr_sect != LINK)
        continue;
      int ret = link_key_value(server, links, i, curr_link);
      if (ret == -1)
        return -1;
    }
  }
  server->links = links;
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
      free(line);
      free(tmp);
      continue;
    }
    tmp2 = parse_line_by_line(tmp);
    if (!tmp2[0])
    {
      free(line);
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
  if (store_link_values(server))
    return 1;
  for (int i = 0; i < server->link_count; i++)
  {
    printf("name is: %s\n", server->links[i].name);
    printf("port is: %d\n", server->links[i].port);
    printf("password is: %s\n", server->links[i].password);
  }
  close(file_fd);
  ft_free(arr);
  exit(1);
  return 0;
}

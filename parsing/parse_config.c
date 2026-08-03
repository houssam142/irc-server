#include "../includes/parse.h"

char *parse_line_by_line(char *line)
{
  size_t i;
  char *tmp;
  char *copy;

  i = 0;
  tmp = malloc(ft_strlen(line) + 1);
  if (!tmp)
  {
    perror("malloc");
    return NULL;
  }
  copy = tmp;
  while (i < ft_strlen(line))
  {
    if (line[i] == '#')
    {
      if (i)
      {
        copy = realloc(tmp, i - 1);
        if (!copy)
          return NULL;
      }
      while (i < ft_strlen(line))
        i++;
      continue;
    }
    copy[i] = line[i];
    i++;
  }
  return copy;
}

int parse_config(char *file)
{
  char *line = NULL;
  int file_fd = open(file, O_RDONLY, 0400);
  if (file_fd < 0)
  {
    perror("open");
    return -1;
  }
  while ((line = get_next_line(file_fd)) != NULL)
  {
    char *tmp = ft_strtrim(line, " \t\n");
    line = ft_strdup(tmp);
    free(tmp);
    line = parse_line_by_line(line);
    if (!line)
      return -1;
    free(line);
  }
  return 0;
}

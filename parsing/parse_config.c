#include "../includes/parse.h"

char *parse_line_by_line(char *line)
{
  int i;
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
  while (i < (int)ft_strlen(line))
  {
    if (line[i] == '#')
    {
      if (i)
      {
        copy = realloc(tmp, i - 1);
        if (!copy)
          return NULL;
      }
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

int parse_config(char *file)
{
  char *line = NULL;
  int line_count = 0;
  int i = 0;
  char **arr = NULL;
  int file_fd = open(file, O_RDONLY, 0400);
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
      continue;
    arr[i] = parse_line_by_line(tmp);
    i++;
  }
  close(file_fd);
  return 0;
}

#include "../includes/parse.h"

char *handle_nickname(char *buffer, t_client *cls)
{
  char *tmp = ft_substr(buffer, ft_strlen("NICK") + 1, ft_strlen(buffer));
  char *arg = ft_strtrim(tmp, " \t");
  if (!arg[0])
  {
    fprintf(stderr, "<NICK>: needs one argument");
    return NULL;
  }
  free(tmp);
  char **words = ft_split(arg, ' ');
  if (!words)
  {
    fprintf(stderr, "ft_split failed due to...\n");
    return NULL;
  }
  int arr_len = get_array_size(words);
  if (arr_len > 1)
  {
    fprintf(stderr, "<NICK>: needs one argument\n");
    return NULL;
  }
  return arg;
}

bool parse_input(char *buff, t_client *cls)
{
  char *nick;
  char *buff_trimed = ft_strtrim(buff, " \t");
  if (!ft_strncmp(buff_trimed, "NICK", 4))
  {
    nick = handle_nickname(buff_trimed, cls);
    if (!nick)
      return false;
  }
  else if (!ft_strncmp(buff_trimed, "USER", 4))
  {

  }
  create_client(cls);
  free(buff_trimed);
  return true;
}

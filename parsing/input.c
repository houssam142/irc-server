#include "../includes/parse.h"
#include "../includes/server.h"

char *handle_nickname(char *buffer, t_client cls[], int client_fd)
{
  char *tmp = ft_substr(buffer, ft_strlen("NICK") + 1, ft_strlen(buffer));
  char *arg = ft_strtrim(tmp, " \t\n");
  free(tmp);
  if (!arg[0])
  {
    cls[client_fd].error_message = ft_strdup("<NICK>: needs exactly one argument\n");
    free(arg);
    return NULL;
  }
  char **words = ft_split(arg, ' ');
  if (!words)
  {
    fprintf(stderr, "ft_split failed due to...\n");
    free(arg);
    return NULL;
  }
  int arr_len = get_array_size(words);
  if (arr_len > 1)
  {
    cls[client_fd].error_message = ft_strdup("<NICK>: needs exactly one argument\n");
    ft_free(words);
    free(arg);
    return NULL;
  }
  ft_free(words);
  for (int i = 0; i <= max_file_descriptors; i++)
  {
    if (cls[i].nick != NULL && !strcmp(cls[i].nick, arg))
    {
      char m[64];
      ft_bzero(m, sizeof(m));
      sprintf(m, "Error: current client has the same nickname as client %d\n", cls[i].fd);
      cls[client_fd].error_message = ft_strdup(m);
      free(arg);
      return NULL;
    }
  }
  return ft_strtrim(arg, "\t");
}

bool parse_input(char *buff, t_client cls[], int client_fd)
{
  char *nick;
  char *buff_trimed = ft_strtrim(buff, " \t");
  if (!ft_strncmp(buff_trimed, "NICK", 4))
  {
    nick = handle_nickname(buff_trimed, cls, client_fd);
    if (!nick)
      return false;
  }
  else if (!ft_strncmp(buff_trimed, "USER", 4))
  {

  }
  else
  {
    cls[client_fd].error_message = ft_strdup("Irc server: Command not supported\n");
    free(buff_trimed);
    return false;
  }
  cls[client_fd].nick = ft_strdup(nick);
  free(nick);
  free(buff_trimed);
  return true;
}

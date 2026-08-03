#include "../includes/parse.h"
#include "../includes/server.h"

bool  handle_username(t_client cls[], int client_fd, char *buffer)
{
  char *tmp = ft_substr(buffer, ft_strlen("USER") + 1, ft_strlen(buffer));
  char *args = ft_strtrim(tmp, " \t\n");
  free(tmp);
  if (!args[0])
  {
    cls[client_fd].error_message = ft_strdup("<USER>: needs exactly three arguments <username>, <hostname> and <realname>\n");
    free(args);
    return false;
  }
  char **words = ft_split(args, ' ');
  if (!words)
  {
    fprintf(stderr, "ft_split failed due to...\n");
    free(args);
    return false;
  }
  if (get_array_size(words) != 3)
  {
    cls[client_fd].error_message = ft_strdup("<USER>: needs exactly three arguments <username>, <hostname> and <realname>\n");
    ft_free(words);
    free(args);
    return false;
  }
  cls[client_fd].username = ft_strdup(words[0]);
  cls[client_fd].hostname = ft_strdup(words[1]);
  cls[client_fd].real_name = ft_strdup(words[2]);
  ft_free(words);
  printf("client username: %s\n", cls[client_fd].username);
  printf("client hostname: %s\n", cls[client_fd].hostname);
  printf("client real_name: %s\n", cls[client_fd].real_name);
  return true;
}

void broadcast(int client_fd, int except_fd)
{
  ssize_t n;
  char msg[64];

  sprintf(msg, "Client %d disconnected from the server\n", except_fd);
  n = send(client_fd, msg, ft_strlen(msg), 0);
  if (n < 0)
  {
    fprintf(stderr, "Failed to send msg to the client(s)\n");
    return ;
  }
}

int quit_command(t_client cls[], int client_fd)
{
  free_client(cls, client_fd);
  for (int i = 0; i <= max_file_descriptors; i++)
  {
    if (cls[i].fd != 0 && cls[i].fd != -1 && cls[i].fd != client_fd)
      broadcast(cls[i].fd, client_fd);
  }
  return 0;
}

bool  handle_nickname(char *buffer, t_client cls[], int client_fd)
{
  char *tmp = ft_substr(buffer, ft_strlen("NICK") + 1, ft_strlen(buffer));
  char *arg = ft_strtrim(tmp, " \t\n");
  free(tmp);
  if (!arg[0])
  {
    cls[client_fd].error_message = ft_strdup("<NICK>: needs exactly one argument\n");
    free(arg);
    return false;
  }
  char **words = ft_split(arg, ' ');
  if (!words)
  {
    fprintf(stderr, "ft_split failed due to...\n");
    free(arg);
    return false;
  }
  int arr_len = get_array_size(words);
  if (arr_len > 1)
  {
    cls[client_fd].error_message = ft_strdup("<NICK>: needs exactly one argument\n");
    ft_free(words);
    free(arg);
    return false;
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
      return false;
    }
  }
  cls[client_fd].nick = ft_strtrim(arg, " \t");
  return true;
}

bool handle_password(t_client cls[], int client_fd, char *buff)
{
  char *tmp = ft_substr(buff, ft_strlen("PASS") + 1, ft_strlen(buff));
  char *arg = ft_strtrim(tmp, " \t\n");
  free(tmp);
  if (!arg[0])
  {
    cls[client_fd].error_message = ft_strdup("<PASS>: needs only one argument\n");
    free(arg);
    return false;
  }
  char **words = ft_split(arg, ' ');
  if (!words)
  {
    fprintf(stderr, "ft_split failed due to...\n");
    free(arg);
    return false;
  }
  if (get_array_size(words) != 1)
  {
    cls[client_fd].error_message = ft_strdup("<PASS>: needs only one argument\n");
    ft_free(words);
    free(arg);
    return false;
  }
  ft_free(words);
  if (strcmp(arg, cls[client_fd].server->password))
  {
    cls[client_fd].error_message = ft_strdup("incorrect password, try again\n");
    free(arg);
    return false;
  }
  cls[client_fd].is_matched = true;
  cls[client_fd].server_password = ft_strdup(arg);
  free(arg);
  return true;
}

int parse_input(char *buff, t_client cls[], int client_fd)
{
  char  *buff_trimed = ft_strtrim(buff, " \t");
  if (!ft_strncmp(buff_trimed, "PASS", 4))
  {
    bool isMatched = handle_password(cls, client_fd, buff_trimed);
    if (!isMatched)
      return 1;
  }
  else if (!ft_strncmp(buff_trimed, "NICK", 4))
  {
    if (!cls[client_fd].is_matched)
    {
      cls[client_fd].error_message = ft_strdup("Can't execute command: not connected to the server\n");
      return 1;
    }
    if (!handle_nickname(buff_trimed, cls, client_fd))
      return 1;
  }
  else if (cls[client_fd].is_matched && !ft_strncmp(buff_trimed, "USER", 4))
  {
    if (!cls[client_fd].is_matched)
    {
      cls[client_fd].error_message = ft_strdup("Can't execute command: not connected to the server\n");
      return 1;
    }
    if (!handle_username(cls, client_fd, buff_trimed))
      return 1;
  }
  else if (cls[client_fd].is_matched && !ft_strncmp(buff_trimed, "QUIT", 4))
  {
    if (!cls[client_fd].is_matched)
    {
      cls[client_fd].error_message = ft_strdup("Can't execute command: not connected to the server\n");
      return 1;
    }
    if (!quit_command(cls, client_fd))
      return 2;
  }
  else
  {
    cls[client_fd].error_message = ft_strdup("Irc server: Command not supported\n");
    free(buff_trimed);
    return 1;
  }
  free(buff_trimed);
  return 0;
}

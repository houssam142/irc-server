#include "../includes/parse.h"
#include "../includes/server.h"

bool  handle_username(t_client *cls, int client_fd, char *buffer)
{
  char *tmp = NULL;
  char *pos = strstr(buffer, "\r\n");
  if (!pos)
    tmp = ft_substr(buffer, ft_strlen("USER") + 1, ft_strlen(buffer));
  else
    tmp = ft_substr(buffer, ft_strlen("USER") + 1, ft_strlen(&buffer[ft_strlen("USER") + 1]) - ft_strlen(pos));
  char *args = ft_strtrim(tmp, " \t\n");
  free(tmp);
  if (!args[0])
  {
    cls[client_fd].error_message = ft_strdup("<USER>: needs exactly four arguments <username>, <hostname>, <servername> and <realname>\n");
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
  if (get_array_size(words) != 4)
  {
    cls[client_fd].error_message = ft_strdup("<USER>: needs exactly four arguments <username>, <hostname>, <servername> and <realname>\n");
    ft_free(words);
    free(args);
    return false;
  }
  cls[client_fd].username = ft_strdup(words[0]);
  cls[client_fd].hostname = ft_strdup(words[1]);
  cls[client_fd].server_name = ft_strdup(words[2]);
  cls[client_fd].real_name = ft_strdup(words[3]);
  ft_free(words);
  return true;
}

void broadcast(int client_fd, int except_fd)
{
  ssize_t n;
  char msg[64];

  sprintf(msg, "\nClient %d disconnected from the server\n", except_fd);
  n = send(client_fd, msg, ft_strlen(msg), 0);
  if (n < 0)
  {
    fprintf(stderr, "\nFailed to send msg to the client(s)\n");
    return ;
  }
}

int quit_command(t_server *server, int client_fd)
{
  for (size_t i = 0; i < server->client_count; i++)
  {
    if (server->cls[i].fd != client_fd)
      broadcast(server->cls[i].fd, client_fd);
  }
  free_client(server, client_fd);
  return 0;
}

bool  handle_nickname(char *buffer, t_client *cls, int client_fd)
{
  char *tmp = NULL;
  char *pos = strstr(buffer, "\r\n");
  if (!pos)
    tmp = ft_substr(buffer, ft_strlen("NICK") + 1, ft_strlen(buffer));
  else
    tmp = ft_substr(buffer, ft_strlen("NICK") + 1, ft_strlen(&buffer[ft_strlen("NICK") + 1]) - ft_strlen(pos));
  char *arg = ft_strtrim(tmp, " \r\t\n\"");
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
  cls[client_fd].nick = ft_strdup(arg);
  return true;
}

bool handle_password(t_client *cls, int client_fd, char *buff)
{
  char *tmp = NULL;
  char *pos = strstr(buff, "\r\n");
  if (!pos)
    tmp = ft_substr(buff, ft_strlen("PASS") + 1, ft_strlen(buff));
  else
    tmp = ft_substr(buff, ft_strlen("PASS") + 1, ft_strlen(&buff[ft_strlen("PASS") + 1]) - ft_strlen(pos));
  char *arg = ft_strtrim(tmp, " \r\t\n\"");
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
  if (strcmp(arg, cls[client_fd].server_side->password))
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

void  propagate_client_data_to_other_servers(t_server *server, int except_fd)
{
  for (size_t i = 0; i < server->client_count; i++)
  {
    if (server->cls[i].fd != except_fd && server->cls[i].conn_type == SERVER_CONNECTED)
    {
      
    }
  }
}

int change_conn_type(char *buff, t_client *cls, int client_fd)
{
  char *tmp = ft_strtrim(buff, "\r\n");
  char *value = ft_substr(tmp, ft_strlen("SERVER") + 1, ft_strlen(tmp));
  if (!tmp || !value)
    return -1;
  for (int i = 0; i < cls[client_fd].server_side->link_count; i++)
  {
    if (!strcmp(cls[client_fd].server_side->links[i].name, value))
    {
      cls[client_fd].conn_type = SERVER_CONNECTED;
      break;
    }
  }
  free(value);
  free(tmp);
  return 0;
}

int parse_input(char *buff, t_server *server, int client_fd)
{
  char *buff_trimed = NULL;
  for (size_t i = 0; i < server->client_count; i++)
  {
    if (server->cls[i].fd == client_fd)
    {
      buff_trimed = ft_strtrim(buff, " \t");
      if (ft_strlen(buff_trimed) <= 2)
      {
        free(buff_trimed);
        return 4; // this is for CRLF, because nc command with the flag -C keeps sending a second buffer containing only "\r\n"
      }
      if (!ft_strncmp(buff_trimed, "INTRO", 5))
      {

      }
      else if (!ft_strncmp(buff_trimed, "PASS", 4) && !server->cls[i].is_matched)
      {
        bool isMatched = handle_password(server->cls, i, buff_trimed);
        if (!isMatched)
        {
          free(buff_trimed);
          return 1;
        }
        if (server->cls[i].is_matched)
        {
          if (!ft_strncmp(&buff_trimed[ft_strnstr(buff_trimed, "\r\n", ft_strlen(buff_trimed)) + 2], "SERVER", 6))
          {
            if (change_conn_type(&buff_trimed[ft_strnstr(buff_trimed, "\r\n", ft_strlen(buff_trimed)) + 2], server->cls, i))
              return -1;
            propagate_client_data_to_other_servers(server, client_fd);
          }
          fprintf(stdout, "Attempt to enter the IRC server was successfull\n");
          server->cls[i].error_message = ft_strdup("Password is correct, Welcome to IRC server\n");
          free(buff_trimed);
          memset(server->buff_recv, 0, sizeof(server->buff_recv));
          return 1;
        }
      }
      else if (!server->cls[i].is_matched)
      {
        server->cls[i].error_message = ft_strdup("Can't execute command: not connected to the server\n");
        free(buff_trimed);
        return 1;
      }
      else if (!ft_strncmp(buff_trimed, "NICK", 4))
      {
        if (!handle_nickname(buff_trimed, server->cls, i))
        {
          free(buff_trimed);
          return 1;
        }
        memset(server->buff_recv, 0, sizeof(server->buff_recv));
        if (!server->cls[i].registered)
        {
          if (server->cls[i].nick && server->cls[i].username && server->cls[i].hostname
            && server->cls[i].server_name && server->cls[i].real_name)
          {
            if (server->cls[i].nick[0] && server->cls[i].username[0] && server->cls[i].hostname[0]
              && server->cls[i].server_name[0] && server->cls[i].real_name[0])
            {
              char m[64];
              bzero(m, sizeof(m));
              sprintf(m, "Client %lu has registered successfully\n", server->client_id);
              server->cls[i].error_message = ft_strdup(m);
              server->cls[i].registered = true;
              free(buff_trimed);
              memset(server->buff_recv, 0, sizeof(server->buff_recv));
              return 1;
            }
          }
        }
      }
      else if (server->cls[i].is_matched && !ft_strncmp(buff_trimed, "USER", 4))
      {
        if (!handle_username(server->cls, i, buff_trimed))
        {
          free(buff_trimed);
          return 1;
        }
        memset(server->buff_recv, 0, sizeof(server->buff_recv));
        if (!server->cls[i].registered)
        {
          if (server->cls[i].nick && server->cls[i].username && server->cls[i].hostname
            && server->cls[i].server_name && server->cls[i].real_name)
          {
            if (server->cls[i].nick[0] && server->cls[i].username[0] && server->cls[i].hostname[0]
              && server->cls[i].server_name[0] && server->cls[i].real_name[0])
            {
              char m[64];
              bzero(m, sizeof(m));
              sprintf(m, "Client %lu has registered successfully\n", i);
              server->cls[i].error_message = ft_strdup(m);
              server->cls[i].registered = true;
              free(buff_trimed);
              memset(server->buff_recv, 0, sizeof(server->buff_recv));
              return 1;
            }
          }
        }
      }
      else if (server->cls[i].is_matched && !ft_strncmp(buff_trimed, "QUIT", 4))
      {
        if (!quit_command(server, server->cls[i].fd))
          return 2;
      }
      else
      {
        server->cls[i].error_message = ft_strdup("Irc server: Command not supported\n");
        memset(server->buff_recv, 0, sizeof(server->buff_recv));
        free(buff_trimed);
        return 1;
      }
      free(buff_trimed);
    }
  }
  return 0;
}

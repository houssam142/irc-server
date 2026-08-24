#include "includes/server.h"


void  free_client(t_server *server, int client_fd)
{
  for (size_t i = 0; i < server->client_count; i++)
  {
    if (server->cls[i].fd == client_fd)
    {
      if (server->cls[i].nick)
      {
        free(server->cls[i].nick);
        server->cls[i].nick = NULL;
      }
      if (server->cls[i].username)
      {
        free(server->cls[i].username);
        server->cls[i].nick = NULL;
      }
      if (server->cls[i].hostname)
      {
        free(server->cls[i].hostname);
        server->cls[i].hostname = NULL;
      }
      if (server->cls[i].real_name)
      {
        free(server->cls[i].real_name);
        server->cls[i].real_name = NULL;
      }
      if (server->cls[i].error_message)
      {
        free(server->cls[i].error_message);
        server->cls[i].error_message = NULL;
      }
      close(client_fd);
      server->cls[i].fd = -1;
      server->cls[i].registered = false;
      server->cls[i].is_matched = false;
      server->client_id--;
      memset(server->buff_recv, 0, sizeof(server->buff_recv));
    }
  }
}

bool start_message(char *msg, int client_fd)
{
  ssize_t n;

  n = send(client_fd, msg, ft_strlen(msg), 0);
  if (n <= 0)
  {
    fprintf(stderr, "Failed to send the starting message to the client\n");
    return false;
  }
  return true;
}

int accept_connections(int epoll_fd, int sockfd, epoll_t events[], t_server *server)
{
  epoll_t client_event;
  int new_fd = accept(sockfd, NULL, NULL);
  if (new_fd < 0)
  {
    perror("accept");
    return -1;
  }
  char *start_msg = "Please enter your command: ";
  client_event.events = EPOLLIN;
  client_event.data.fd = new_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &client_event) < 0)
  {
    perror("epoll_ctl");
    return -1;
  }
  if (server->client_count >= server->client_cap)
  {
    server->client_cap *= 2;
    t_client *tmp = realloc(server->cls, server->client_cap * sizeof(*server->cls));
    if (!server->cls)
    {
      perror("realloc");
      exit(1);
    }
    server->cls = tmp;
  }
  server->cls[server->client_count].nick = NULL;
  server->cls[server->client_count].username = NULL;
  server->cls[server->client_count].hostname = NULL;
  server->cls[server->client_count].server_name = NULL;
  server->cls[server->client_count].real_name = NULL;
  server->cls[server->client_count].fd = new_fd;
  server->cls[server->client_count].server_password = NULL;
  server->cls[server->client_count].error_message = NULL;
  server->cls[server->client_count].is_matched = false;
  server->cls[server->client_count].conn_type = CLIENT_CONNECTED;
  server->cls[server->client_count].server_side = server;
  server->cls[server->client_count].registered = false;
  events[new_fd] = client_event;
  server->client_count++;
  server->client_id++;
  fprintf(stdout, "New connection %d accepted\n", new_fd);
  if (!start_message(start_msg, new_fd))
    return -1;
  return 0;
}

int receive_data(int client_fd, t_server *server)
{
  ssize_t bytes;
  char    buff[2048];
  int parse_flag;

  memset(buff, 0, sizeof(buff));
  bytes = recv(client_fd, buff, sizeof(buff), 0);
  if (bytes <= 0)
  {
    fprintf(stderr, "client %d left the server\n", client_fd);
    free_client(server, client_fd);
    return -1;
  }
  strcat(server->buff_recv, buff);
  int pos = ft_strnstr(server->buff_recv, "\r\n", ft_strlen(server->buff_recv));
  if (pos < 0)
    return 2;
  switch ((parse_flag = parse_input(server->buff_recv, server, client_fd))) {
    case 1:
      return 1;
    case 2:
      return -1;
    case 4:
      return 4;
    default:
      break;
  }
  return 0;
}

int send_response(int client_fd, t_server *server)
{
  ssize_t n;

  for (size_t i = 0; i < server->client_count; i++)
  {
    if (server->cls[i].fd == client_fd)
    {
      if (!server->cls[i].error_message)
        return 1;
      n = send(client_fd, server->cls[i].error_message, ft_strlen(server->cls[i].error_message), 0);
      if (n < 0)
      {
        free_client(server, client_fd);
        close(client_fd);
        return -1;
      }
      if (n == 0)
      {

      }
    }
  }
  return 0;
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		puts("Wrong number of arguments: <irc config file>\n");
		return 1;
	}

  t_server  server;
  if (parse_config(av[1], &server))
    return 8;
  t_client *cls = malloc(INITIAL_CLIENT_CAPACITY * sizeof(t_client));
  if (!cls)
  {
    perror("malloc");
    return 17;
  }
  server.cls = cls;
  server.client_count = 0;
  server.client_id = 0;
  server.client_cap = INITIAL_CLIENT_CAPACITY;
	struct sockaddr_in addr;
  int out_socket;
	epoll_t event, events[MAX_EVENTS];
  epoll_t another_event[server.link_count - 1];
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket");
		return 1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(ft_itoa(server.port)));
	addr.sin_addr.s_addr = INADDR_ANY;
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		return 2;
	}
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return 3;
	}
	if (listen(sockfd, 128) < 0)
	{
		perror("listen");
		return 4;
	}
  fcntl(sockfd, F_SETFL, O_NONBLOCK);
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1)
  {
    perror("epoll create");
    close(sockfd);
    return 5;
  }
  event.events = EPOLLIN;
  event.data.fd = sockfd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) < 0)
  {
    fprintf(stderr, "Error adding %d to epoll event\n", sockfd);
    return 6;
  }
  for (int i = 0; i < server.link_count; i++)
  {
    out_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (out_socket < 0)
    {
      perror("socket");
      return 7;
    }
    server.links[i].fd = -1;
    struct sockaddr_in addr_conn;
    addr_conn.sin_family = AF_INET;
    addr_conn.sin_port = htons(atoi(ft_itoa(server.links[i].port)));
    addr_conn.sin_addr.s_addr = INADDR_ANY;
    int res = connect(out_socket, (struct sockaddr *)&addr_conn, sizeof(addr_conn));
    if (res == 0)
    {
      char *buff = malloc(ft_strlen(server.links[i].password) + ft_strlen(server.links[i].name) + 16);
      if (!buff)
      {
        perror("malloc");
        return 15;
      }
      another_event[i].events = EPOLLIN;
      another_event[i].data.fd = out_socket;
      if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, out_socket, another_event) < 0)
      {
        fprintf(stderr, "Error adding %d to epoll event\n", out_socket);
        return 6;
      }
      sprintf(buff, "PASS %s\r\nSERVER %s\r\n", server.links[i].password, server.links[i].name);
      if (!start_message(buff, out_socket))
      {
        free(buff);
        return 16;
      }
      free(buff);
      server.links[i].fd = out_socket;
      fprintf(stdout, "Server-to-server connection established with fd %d\n", out_socket);
    }
    else if (res == -1)
    {
      if (errno == EINPROGRESS)
      {
        another_event[i].events = EPOLLOUT;
        another_event[i].data.fd = out_socket;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, out_socket, another_event);
        epoll_wait(epoll_fd, another_event, server.link_count, -1);
      }
      else
      {
        perror("connect");
        continue;
      }
    }
  }
  char *start_msg = "Please enter your command: ";
  memset(events, 0, sizeof(epoll_t));
  while (1)
  {
    int event_counts = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (event_counts < 0)
    {
      if (errno == EINTR)
        continue;
      perror("epoll wait");
      break;
    }
    for (int i = 0; i < event_counts; i++)
    {
      if (events[i].data.fd == sockfd && events[i].events & EPOLLIN)
      {
        int ret = accept_connections(epoll_fd, sockfd, events, &server);
        if (ret < 0)
          continue;
      }
      if (events[i].data.fd != sockfd && events[i].events & EPOLLIN)
      {
        int ret = receive_data(events[i].data.fd, &server);
        if (ret == 2)
          continue;
        if (ret < 0)
        {
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &events[events[i].data.fd]);
          continue;
        }
        else if (ret == 1)
        {
          events[i].events = EPOLLOUT;
          epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &events[events[i].data.fd]);
        }
       else if (ret == 4)
        {
          memset(server.buff_recv, 0, sizeof(server.buff_recv));
          goto message;
        }
      }
      if (events[i].data.fd != sockfd && events[i].events & EPOLLOUT)
      {
        int ret = send_response(events[i].data.fd, &server);
        if (ret < 0)
        {
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &events[events[i].data.fd]);
          continue;
        }
        memset(server.buff_recv, 0, sizeof(server.buff_recv));
      }
    message:
      if (events[i].data.fd != sockfd && !start_message(start_msg, events[i].data.fd))
        continue;
    }
  }
  close(sockfd);
  close(epoll_fd);
	return 0;
}

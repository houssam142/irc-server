#include "includes/server.h"

int max_file_descriptors;


void  free_client(t_client cls[], int client_fd)
{
  if (cls[client_fd].nick)
    free(cls[client_fd].nick);
  if (cls[client_fd].username)
    free(cls[client_fd].username);
  if (cls[client_fd].hostname)
    free(cls[client_fd].hostname);
  if (cls[client_fd].real_name)
    free(cls[client_fd].real_name);
  if (cls[client_fd].error_message)
    free(cls[client_fd].error_message);
  close(client_fd);
  cls[client_fd].fd = -1;
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

int accept_connections(int epoll_fd, int sockfd, t_client cls[], epoll_t events[], t_server *server)
{
  epoll_t client_event;
  int new_fd = accept(sockfd, NULL, NULL);
  if (new_fd < 0)
  {
    perror("accept");
    return -1;
  }
  char *start_msg = "Please enter your command: ";
  int flags = fcntl(new_fd, F_GETFL, 0);
  fcntl(new_fd, F_SETFL, flags | O_NONBLOCK);
  client_event.events = EPOLLIN;
  client_event.data.fd = new_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &client_event) < 0)
  {
    perror("epoll_ctl");
    return -1;
  }
  cls[new_fd].nick = NULL;
  cls[new_fd].username = NULL;
  cls[new_fd].hostname = NULL;
  cls[new_fd].real_name = NULL;
  cls[new_fd].fd = new_fd;
  cls[new_fd].server_password = NULL;
  cls[new_fd].error_message = NULL;
  cls[new_fd].server = server;
  cls[new_fd].is_matched = false;
  events[new_fd] = client_event;
  max_file_descriptors++;
  fprintf(stdout, "New connection %d accepted\n", new_fd);
  if (!start_message(start_msg, new_fd))
    return -1;
  return 0;
}

int receive_data(int client_fd, t_client cls[])
{
  ssize_t bytes;
  char    buff[2048];

  memset(buff, 0, sizeof(buff));
  bytes = recv(client_fd, buff, sizeof(buff) - 1, 0);
  if (bytes <= 0)
  {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return 1;
    fprintf(stderr, "client %d left the server\n", client_fd);
    close(client_fd);
    return -1;
  }
  buff[bytes] = '\0';
  //printf("buff: %s\n", buff);
  int parse_flag = parse_input(buff, cls, client_fd);
  if (parse_flag == 1)
    return 1;
  if (parse_flag == 2)
    return -1;
  return 0;
}

int send_response(int client_fd, t_client cls[])
{
  ssize_t n;

  if (!cls[client_fd].error_message)
    return 1;
  n = send(client_fd, cls[client_fd].error_message, ft_strlen(cls[client_fd].error_message), 0);
  if (n < 0)
  {
    free_client(cls, client_fd);
    close(client_fd);
    return -1;
  }
  if (n == 0)
  {

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
  t_client  cls[MAX_CLIENTS];
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
    fprintf(stderr, "Error adding epoll fd to epoll event\n");
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
    struct sockaddr_in addr_conn;
    addr_conn.sin_family = AF_INET;
    addr_conn.sin_port = htons(atoi(ft_itoa(server.links[i].port)));
    addr_conn.sin_addr.s_addr = INADDR_ANY;
    int res = connect(out_socket, (struct sockaddr *)&addr_conn, sizeof(addr_conn));
    if (res == 0)
    {
      char *buff = malloc(ft_strlen(server.links[i].password) + 64);
      if (!buff)
      {
        perror("malloc");
        return 15;
      }
      another_event[i].events = EPOLLIN;
      another_event[i].data.fd = out_socket;
      if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, out_socket, another_event) < 0)
      {
        fprintf(stderr, "Error adding epoll fd to epoll event\n");
        return 6;
      }
      sprintf(buff, "PASS %s\n\r\nSERVER %s\r\n", server.links[i].password, server.links[i].name);
      if (!start_message(buff, out_socket))
        return 16;
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
  memset(cls, 0, sizeof(t_client));
  memset(events, 0, sizeof(epoll_t));
  max_file_descriptors = sockfd;
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
        int ret = accept_connections(epoll_fd, sockfd, cls, events, &server);
        if (ret < 0)
          continue;
      }
      if (events[i].data.fd != sockfd && events[i].events & EPOLLIN)
      {
        int ret = receive_data(events[i].data.fd, cls);
        if (ret < 0)
        {
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &events[events[i].data.fd]);
          continue;
        }
        if (ret == 1)
        {
          events[i].events = EPOLLOUT;
          epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &events[events[i].data.fd]);
        }
      }
      if (events[i].data.fd != sockfd && events[i].events & EPOLLOUT)
      {
        int ret = send_response(events[i].data.fd, cls);
        if (ret < 0)
        {
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &events[events[i].data.fd]);
          continue;
        }
      }
      if (events[i].data.fd != sockfd && !start_message(start_msg, events[i].data.fd))
        continue;
    }
  }
  close(epoll_fd);
	return 0;
}

#include "includes/server.h"

int accept_connections(int epoll_fd, int sockfd)
{
  epoll_t client_event;
  int new_fd = accept(sockfd, NULL, NULL);
  if (new_fd < 0)
  {
    perror("accept");
    return -1;
  }
  t_client  cls;
  int flags = fcntl(new_fd, F_GETFL, 0);
  fcntl(new_fd, F_SETFL, flags | O_NONBLOCK);
  memset(&cls, 0, sizeof(t_client));
  client_event.events = EPOLLIN;
  client_event.data.fd = new_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &client_event) < 0)
  {
    perror("epoll_ctl");
    return -1;
  }
  fprintf(stdout, "New connection %d accepted\n", new_fd);
  return 0;
}

int receive_data(int client_fd)
{
  ssize_t bytes;
  char    buff[1024];

  memset(buff, 0, sizeof(buff));
  bytes = recv(client_fd, buff, sizeof(buff) - 1, 0);
  if (bytes <= 0)
  {
    close(client_fd);
    return -1;
  }
  buff[bytes] = '\0';
  bool parse_flag = parse_input(buff, &cls);
  if (!parse_flag)
    return 1;
  return 0;
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		puts("Wrong number of arguments\n");
		return 1;
	}
	struct sockaddr_in addr;
	epoll_t event, events[MAX_EVENTS];
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket");
		return 1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(av[1]));
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
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1)
  {
    perror("epoll create");
    close(sockfd);
    return 5;
  }
  memset(events, 0, sizeof(epoll_t));
  event.events = EPOLLIN;
  event.data.fd = sockfd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) < 0)
  {
    fprintf(stderr, "Error adding epoll fd to epoll event\n");
    return 6;
  }
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
        int ret = accept_connections(epoll_fd, sockfd);
        if (ret < 0)
          continue;
      }
      else if (events[i].data.fd != sockfd && events[i].events & EPOLLIN)
      {
        int ret = receive_data(events[i].data.fd);
        if (ret < 0)
          continue;
        i++;
        continue;
      }
    }
  }
  close(epoll_fd);
	return 0;
}

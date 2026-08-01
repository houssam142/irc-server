#pragma once

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/epoll.h>
# include <stdbool.h>
# include <errno.h>
# include <fcntl.h>

#define MAX_EVENTS 100

typedef struct epoll_event epoll_t;

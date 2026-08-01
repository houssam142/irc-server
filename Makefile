SRCS = server.c

CC = cc

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g

EXEC = irc_server

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(EXEC)

re: fclean all

ra: all clean

.PHONY: all clean fclean all ra


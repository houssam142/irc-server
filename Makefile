SRCS = server.c \
        parsing/input.c \
	parsing/parse_utils.c

CC = cc

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g

EXEC = irc_server

LIBFT_DIR = ./libft

LIBFT_LIB = $(LIBFT_DIR)/libft.a

all: $(EXEC)

$(EXEC): $(OBJS)
	@make -s -C ./libft
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) -o $(EXEC)

clean:
	@make -s fclean -C ./libft
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(EXEC)

re: fclean all

ra: all clean

.PHONY: all clean fclean all ra


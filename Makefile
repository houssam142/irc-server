SRCS = server.c \
        parsing/input.c \
				parsing/parse_utils.c \
				parsing/parse_config.c \
				utils/get_next_line.c \
				utils/get_next_line_utils.c

CC = gcc

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g -no-pie

EXEC = irc_server

LIBFT_DIR = ./libft

LIBFT_LIB = $(LIBFT_DIR)/libft.a

all: $(EXEC)

$(EXEC): $(OBJS)
	@make -s -C ./libft
	$(CC) $(OBJS) -no-pie $(LIBFT_LIB) -o $(EXEC)
%o: %.c
	$(CC) $(CFLAG) -c $< -o $@

clean:
	@make -s fclean -C ./libft
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(EXEC)

re: fclean all

ra: all clean

.PHONY: all clean fclean all ra


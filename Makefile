NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
SRCS = parser.c
OBJS = 
INCDIR = minishell.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(INCDIR)
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
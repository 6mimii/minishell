USER = JhoinZ

LIB_RDL = -lreadline

LIBFT_DIR = Libft/

LIBFT = libft.a

SRCS_DIR = src

OBJ_DIR = obj

FILES = src/main.c \
src/parser/init.c \
src/parser/tokens.c \
src/parser/set_tokens.c \
src/parser/nodes.c \
src/parser/enviroment.c \
src/parser/commands.c \
src/executor/executor.c \
src/executor/cd.c \
src/executor/echo.c \
src/executor/env.c \
src/executor/exit.c \
src/executor/export.c \
src/executor/multiple_cmd.c \
src/executor/pwd.c \
src/executor/single_cmd.c \
src/executor/unset.c \
src/utils/utils_1.c \
src/utils/utils_a.c \
src/utils/utils_b.c \
src/utils/error.c \

CC = clang

CFLAGS = -g -Wall -Werror -Wextra

OBJS_SRCS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(FILES))

NAME = minishell

INCLUDE = -I $(SRCS_DIR) -I $(LIBFT_DIR)

RM = rm -rf

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

#Rules

$(NAME): $(OBJS_SRCS)
	@$(MAKE) -s all bonus printf gnl -C $(LIBFT_DIR)
	@$(CC) $(INCLUDE) $(OBJS_SRCS) $(LIB_RDL) -L$(LIBFT_DIR) -lft -o $(NAME)

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

USER = JhoinZ

LIB_RDL = -L/opt/homebrew/opt/readline/lib -lreadline
LIBFT_DIR = Libft/
LIBFT = libft.a
SRCS_DIR = src
OBJ_DIR = obj

FILES = src/main.c \
src/parser/init.c \
src/parser/tokens.c \
src/parser/set_tokens.c \
src/parser/set_tokens_utils.c \
src/parser/nodes.c \
src/parser/enviroment.c \
src/parser/commands.c \
src/parser/command_utils.c \
src/parser/expand_flag.c \
src/parser/expand.c \
src/parser/free_functions.c \
src/parser/here_doc_utils.c \
src/parser/here_doc.c \
src/parser/heredoc_utils_b.c \
src/parser/join_tokens.c \
src/parser/redirections.c \
src/parser/signals.c \
src/executor/executor.c \
src/executor/executor_utils.c \
src/executor/cd.c \
src/executor/echo.c \
src/executor/env.c \
src/executor/exit.c \
src/executor/export.c \
src/executor/export_utils.c \
src/executor/multiple_cmd.c \
src/executor/pwd.c \
src/executor/single_cmd.c \
src/executor/unset.c \
src/utils/expand_utils.c  \
src/utils/expand_utils_b.c \
src/utils/utils_a.c \
src/utils/utils_b.c \
src/utils/utils_c.c \
src/utils/error.c \
src/utils/free_utils.c \
src/utils/command_utils_b.c \
src/utils/command_utils.c \
src/utils/unset_utils.c \
src/utils/unset_utils_b.c \
src/utils/unset_utils_c.c \

CC = clang
CFLAGS = -g -Wall -Werror -Wextra
OBJS_SRCS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(FILES))
NAME = minishell
INCLUDE = -I $(SRCS_DIR) -I $(LIBFT_DIR) -I /opt/homebrew/opt/readline/include
RM = rm -rf

# Rules
all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS_SRCS)
	@$(MAKE) -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS_SRCS) $(LIB_RDL) -L $(LIBFT_DIR) -lft -o $(NAME)

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
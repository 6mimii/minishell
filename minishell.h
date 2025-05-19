/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:19 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/20 01:15:49 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H
# include "Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

extern int			g_signal;

# define RST "\033[0m"
# define RED "\033[1;31m"
# define GRN "\033[1;32m"
# define YLW "\033[1;33m"
# define BLU "\033[1;34m"
# define MGT "\033[1;35m"
# define CYN "\033[1;36m"
# define WIT "\033[1;37m"

# define UNEXPECTED_TOK "sintax error near unexpected token"
# define UNEXPECTED_EOF "syntax error unexpected end of file"
# define WRONG_Q "unexpected EOF while looking for matching `''"
# define WRONG_DQ "unexpected EOF while looking for matching `\"'"
# define EXPORT "export: not a valid identifier"
# define MLLC_ERR "Error asignning Malloc"
# define CTRLD_HD \
	"warning: here-document \
delimited by end-of-file (wanted `"

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_G,
	T_L,
	T_DG,
	T_DL,
	T_Q,
	T_DQ,
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	int				flag;
	int				exp;
	int				backslash;
	struct s_token	*next;

}					t_token;

typedef struct s_cmd
{
	char			**argv;
	int				error;
	int				fd_in;
	int				fd_out;
	int				index;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_msh
{
	int				cmd_len;
	int				state;
	char			*input;
	char			**envp;
	char			**path;
	int				parse_error;
	struct s_cmd	*cmd;
	struct s_token	*tokens;
	struct s_env	*env;
}					t_msh;

typedef struct s_env
{
	char			*type;
	char			*content;
	int				index;
	struct s_env	*next;
}					t_env;

extern int			g_signal;

void				init_msh(char **envp, t_msh *msh);
t_env				*enviroment_lst(char **envp);
void				get_input(t_msh *msh);

/////////////TOKENIZER//////////////
t_token				*set_tokens(char *line, t_msh *msh);
void				set_word_token(char *line, int *i, t_token **tokens);
void				set_greather_token(char *line, int *i, t_token **tokens);
void				set_lower_token(char *line, int *i, t_token **tokens);
void				set_pipe_token(char *line, int *i, t_token **tokens);
void				set_quote_token(char *line, int *i, t_token **tokens,
						t_msh *msh);
void				set_double_quote_token(char *line, int *i, t_token **tokens,
						t_msh *msh);
/////////////UTILS//////////////
void				classify_token(char *token);
int					ft_strcmp(char *s1, char *s2);
char				*ft_strchr(const char *s, int c);
char				*my_strtok(char *str, const char *delim);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strdup(const char *s1);
size_t				ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t				ft_strlen(const char *s);
/////////////NODES//////////////
void				add_node_back(t_token **lst, t_token *new);
t_token				*new_node(char *content, int type, int flag);
void				create_token_lst(t_token **tok, int type, char *content,
						int flag);
t_cmd				*new_node_command(void);
int					command_content(t_cmd *new, t_token *tok);
/////////////COMMANDS//////////////
t_cmd				*new_node_cmd(void);
int					cmd_content(t_cmd *new, t_token *tok);
void				create_cmd_lst(t_cmd **cmd, t_cmd *new);
void				get_command(t_msh *msh);
void				create_command_list(t_cmd **cmd, t_cmd *new);
/////////////ENVIRONMENT//////////////
t_env				*env_lst(char **envp);
t_env				*create_env_lst(char **envp);
////////////////SET/////////////////
void				set_outfile(t_token **tok, t_cmd *new, t_msh *msh);
void				set_append(t_token **tok, t_cmd *new, t_msh *msh);
void				set_infile(t_token **tok, t_cmd *new, t_msh *msh);
////////////////FLAGS/////////////////
void				expand_flag(t_token *tok);
////////////////HERE-DOC/////////////////
void				set_heredoc(t_token **tok, t_cmd *new, t_msh *msh);
void				ctrl_c_hd(int signal);
char				*exp_line(char *str, t_msh *msh);
char				*expand_heredoc(char *line, t_msh *msh);
void				free_and_exit_hd(t_msh *msh, t_cmd *new, int state);
////////////////EXPAND/////////////////
char				*no_expand_var(char *s1, int *i);
void				expand_content(t_token *tok, t_msh *msh);
void				expand_tokens(t_token **tokens, t_msh *msh);
int					check_dollar(const char *str);
int					check_home(const char *str);
char				*get_word(char *s1, int *i);
char				*get_exp(char *line, int *i, t_msh *msh);
char				*expand_var(char *var, t_msh *msh, int len);
////////////////FREE/////////////////
void				free_tokens(t_token **tokens);
void				free_commands(t_cmd **cmd);
void				free_matrix(char **matrix);
void				free_msh(t_msh *msh);
void				free_env(t_env *env);

////////////////SIGNALS/////////////////
void				setup_signals(t_msh *msh);
void				ctrl_d(void);

/* Executor */
void				executor(t_msh *msh);
void				set_env(t_msh *msh, char *var, char *new);
void				add_env(t_msh *msh, char *var, char *content);
char				*get_env(t_msh *msh, char *var);
char				*get_env_type(t_msh *msh, char *var);
void				print_export(t_env *env, int fd);

/* Builts in */
int					is_builtin(t_msh *msh, t_cmd *cmd);
void				ft_env(t_msh *msh, t_cmd *cmd, char *next);
void				ft_echo(t_msh *msh, t_cmd *cmd, int fd);
void				ft_pwd(t_msh *msh, t_cmd *cmd);
void				ft_exit(t_msh *msh, t_cmd *cmd);
void				ft_cd(t_msh *msh, t_cmd *cmd);
void				ft_export(t_msh *msh, t_cmd *cmd);
void				ft_unset(t_msh *msh, t_cmd *cmd);

/* One command */
void				handle_single_command(t_msh *msh);
void				run_external_command(t_msh *msh, t_cmd *cmd, char **paths);
char				*find_cmd(char **path, char *cmd, t_msh *msh);
void				error_files(char *name, char *msg);

/* Multiple cmd */
void				multiple_cmds(t_msh *msh, int fd_in);

/* Utils */
char				**get_path(t_msh *msh);
void				set_cmd_ind(t_cmd *cmd);
void				wait_handler(t_msh *msh, pid_t pid);
void				free_and_exit_ex(t_msh *msh);

/* Errors */
void				error_msh(char *msg, t_msh *msh, int state);
void				error_files(char *name, char *msg);
void				error_and_exit(char *name, int state, t_msh *msh);
void				free_and_exit(char *msg, t_msh *msh, int state, bool print);

#endif
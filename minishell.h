/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:58:55 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:24:01 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _POSIX_C_SOURCE 200809L
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
	T_AND,
	T_OR,
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
	char			**original_envp;
	char			**path;
	int				parse_error;
	struct s_cmd	*cmd;
	struct s_token	*tokens;
	struct s_env	*env;
	char			**unset_vars;
}					t_msh;

typedef struct s_env
{
	char			*type;
	char			*content;
	int				index;
	struct s_env	*next;
}					t_env;

void				init_msh(char **envp, t_msh *msh);
t_env				*enviroment_lst(char **envp);
void				get_input(t_msh *msh);
int					clean_tokens(t_msh *msh);
void				reset_msh_for_next_command(t_msh *msh);

/////////////TOKENIZER//////////////
t_token				*set_tokens(char *line, t_msh *msh);
void				set_word_token(char *line, int *i, t_token **tokens);
void				set_greather_token(char *line, int *i, t_token **tokens);
void				set_lower_token(char *line, int *i, t_token **tokens);
void				set_pipe_token(char *line, int *i, t_token **tokens);
void				set_quote_token(char *line, int *i, t_token **tokens);
void				set_double_quote_token(char *line, int *i,
						t_token **tokens);
void				set_logical_operator_token(char *line, int *i,
						t_token **tokens);
int					check_pipes(t_msh *msh, t_token *token, int *flag);
int					check_tokens(t_token **tokens, t_msh *msh, int flag);
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
int					get_command_len(t_token *token);
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
char				*process_heredoc_line(char *line, t_msh *msh, int fd);
void				handle_ctrl_c(t_msh *msh);
void				wait_hd(t_token *tok, t_cmd *cmd, t_msh *msh, int fd);
void				here_doc(char *limit, t_cmd *new, t_msh *msh, int fd);
////////////////EXPAND/////////////////
char				*no_expand_var(char *s1, int *i);
void				expand_content(t_token *tok, t_msh *msh);
void				expand_tokens(t_token **tokens, t_msh *msh);
void				expand_home(t_token *tok, t_msh *msh);
void				expand_both(t_token *tok, t_msh *msh);
int					check_dollar(const char *str);
int					check_home(const char *str);
char				*get_word(char *s1, int *i);
char				*get_exp(char *line, int *i, t_msh *msh);
char				*expand_var(char *var, t_msh *msh, int len);
int					is_special_var(char *var);
char				*handle_special_var(char *var, t_msh *msh);
char				*process_expand_var(char *var, t_msh *msh, char *line);
int					is_special_var(char *var);
char				*handle_special_var(char *var, t_msh *msh);
char				*process_expand_var(char *var, t_msh *msh, char *line);
////////////////FREE/////////////////
void				free_tokens(t_token **tokens);
void				free_commands(t_cmd **cmd);
void				free_matrix(char **matrix);
void				free_msh(t_msh *msh);
void				free_env(t_env *env);

////////////////SIGNALS/////////////////
void				setup_signals(void);
void				ctrl_d(t_msh *msh);

/* Executor */
void				executor(t_msh *msh);
void				set_env(t_msh *msh, char *var, char *new);
void				add_env(t_msh *msh, char *var, char *content);
char				*get_env(t_msh *msh, char *var);
char				*get_env_type(t_msh *msh, char *var);
void				print_export(t_env *env, int fd);
void				print_env_error(char *next, t_msh *msh);

/* Builts in */
int					is_builtin(t_msh *msh, t_cmd *cmd);
void				ft_env(t_msh *msh, t_cmd *cmd, char *next);
void				ft_echo(t_msh *msh, t_cmd *cmd, int fd);
void				ft_pwd(t_msh *msh, t_cmd *cmd);
void				ft_exit(t_msh *msh, t_cmd *cmd);
void				ft_cd(t_msh *msh, t_cmd *cmd);
void				ft_export(t_msh *msh, t_cmd *cmd);
void				ft_unset(t_msh *msh, t_cmd *cmd);
void				exit_extra_options(t_msh *msh, t_cmd *cmd);
void				add_vars(t_msh *msh, char *str);
int					parse_export(t_msh *msh, char *line);
bool				all_nbr(char *line);

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
void				cleanup_envp_array(char **array, int count);
void				process_unset_args(t_msh *msh, t_cmd *cmd, int *changed);
void				add_to_unset_list(t_msh *msh, char *var);
char				**create_envp_array(t_msh *msh);
int					get_unset_vars_length(t_msh *msh, char *var);
int					update_envp_array(t_msh *msh);
char				**create_new_unset_list(t_msh *msh, char *var, int len);
int					count_env_nodes(t_env *env);
char				*create_env_string(t_env *curr);
int					delete_env_var(t_msh *msh, char *var);
void				free_env_node(t_env *env);

/* Errors */
void				error_msh(char *msg, t_msh *msh, int state);
void				error_files(char *name, char *msg);
void				error_and_exit(char *name, int state, t_msh *msh);
void				free_and_exit(char *msg, t_msh *msh, int state, bool print);

/* Command Utils */
int					is_logical_operator(t_token *token);
int					process_command_token(t_cmd *new, t_token *aux, int *i);
void				finalize_command(t_msh *msh, t_cmd *new_command,
						t_token **tokens);
void				process_command_input(t_msh *msh);

#endif
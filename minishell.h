/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:19 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/04/23 16:54:38 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <sys/types.h>
# include <stdbool.h>
# include <errno.h>

/* COLOR */
# define RST					"\033[0m"    // Reset per tornare al colore normale
# define RED					"\033[1;31m" // Rosso brillante
# define GRN					"\033[1;32m" // Verde brillante
# define YLW					"\033[1;33m" // Giallo brillante
# define BLU					"\033[1;34m" // Blu brillante
# define MGT					"\033[1;35m" // Magenta brillante
# define CYN					"\033[1;36m" // Ciano brillante
# define WIT					"\033[1;37m" // Bianco brillante

/* ERROR */
# define	UNEXPECTED_TOK "sintax error near unexpected token"
# define	UNEXPECTED_EOF "syntax error unexpected end of file"
# define	WRONG_Q "unexpected EOF while looking for matching `''"
# define	WRONG_DQ "unexpected EOF while looking for matching `\"'"
# define	EXPORT "export: not a valid identifier"

typedef enum	e_token_type
{
	T_WORD,
	T_PIPE,
	T_G,
	T_L,
	T_DG,
	T_DL,
	T_Q,
	T_DQ,
}				t_token_type;

typedef struct	s_token
{
	t_token_type	type;
	char			*content;
	int				flag;
	struct s_token	*next;
}				t_token;

typedef struct	s_cmd
{
	char			**argv;
	int				error;
	int				fd_in;
	int				fd_out;
	int				index;
	struct s_cmd	*next;
}				t_cmd;

typedef struct	s_msh
{
    int				cmd_len;
	int				state;
    char			*input;
	char			**envp;
    char			**path;
    int				parse_error;
    struct s_token	*tokens;
	struct s_env	*env;
    struct s_cmd	*cmd;
}				t_msh;

typedef struct	s_env
{
	char			*type;
	char			*content;
	int				index;
	struct s_env	*next;
}				t_env;

extern int	g_signal;

void			init_msh(char **envp, t_msh *msh);
void			get_input(t_msh *msh);

/////////////TOKENIZADOR//////////////
t_token			*set_tokens(char *line, t_msh *msh);
static void		set_backslash_token(char *line, int *i, t_token **tokens, int flag);
void			set_word_token(char *line, int *i, t_token **tokens);
void			set_greather_token(char *line, int *i, t_token **tokens);
void			set_lower_token(char *line, int *i, t_token **tokens);
void			set_pipe_token(char *line, int *i, t_token **tokens);
void			set_quote_token(char *line, int *i, t_token **tokens, t_msh *msh);
void			set_double_quote_token(char *line, int *i, t_token **tokens, t_msh *msh);

/////////////UTILS//////////////
void			classify_token(char *token);
int				ft_strcmp(char *s1, char *s2);
char			*ft_strchr(const char *s, int c);
char			*my_strtok(char *str, const char *delim);
char			*ft_substr(char const *s, unsigned int start, size_t len);

/////////////NODES//////////////
void			add_node_back(t_token **lst, t_token *new);
t_token			new_node(t_token **content, int type, int flag);
void			create_token_lst(t_token **tok, int type, char *content, int flag);


t_env			*env_lst(char **envp);
static void		init_env_lst(t_env **lst, char  **envp);

/* Executor */

void			executor(t_msh *msh);
void			set_env(t_msh *msh, char *var, char *new);
void			add_env(t_msh *msh, char *var, char *content);
char			*get_env(t_msh *msh, char *var);
char			*get_env_type(t_msh *msh, char *var);
void			print_export(t_env *env, int fd);

/* Builts in */

int				is_builtin(t_msh *msh, t_cmd *cmd);
void			ft_env(t_msh *msh, t_cmd *cmd, char *next);
void			ft_echo(t_msh *msh, t_cmd *cmd, int fd);
void			ft_pwd(t_msh *msh, t_cmd *cmd);
void			ft_exit(t_msh *msh, t_cmd *cmd);
void			ft_cd(t_msh *msh, t_cmd *cmd);
void			ft_export(t_msh *msh, t_cmd *cmd);
void			ft_unset(t_msh *msh, t_cmd *cmd);

/* One command */

void			handle_single_command(t_msh *msh);
void			run_external_command(t_msh *msh, t_cmd *cmd, char **paths);
char			*find_cmd(char **path, char *cmd, t_msh *msh);

/* Multiple cmd */

void			multiple_cmds(t_msh *msh, int fd_in);

/* Utils */

char			**get_path(t_msh *msh);
void			wait_handler(t_msh *msh, pid_t pid);

/* Errors */
void			error_msh(char *msg, t_msh *msh, int state);
void			error_files(char *name, char *msg);
void			error_and_exit(char *name, int state, t_msh *msh);
void			free_and_exit(char *msg, t_msh *msh, int state, bool print);

#endif
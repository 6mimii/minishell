/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:19 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/03/17 17:35:19 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

/* COLOR */
#define RST                     "\033[0m"    // Reset per tornare al colore normale
#define RED                     "\033[1;31m" // Rosso brillante
#define GRN                     "\033[1;32m" // Verde brillante
#define YLW                     "\033[1;33m" // Giallo brillante
#define BLU                     "\033[1;34m" // Blu brillante
#define MGT                     "\033[1;35m" // Magenta brillante
#define CYN                     "\033[1;36m" // Ciano brillante
#define WIT                     "\033[1;37m" // Bianco brillante

/* ERROR */
#define     UNEXPECTED_TOK "sintax error near unexpected token"
#define     UNEXPECTED_EOF "syntax error unexpected end of file"
#define     WRONG_Q "unexpected EOF while looking for matching `''"
#define     WRONG_DQ "unexpected EOF while looking for matching `\"'"

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
}   t_token_type;


typedef struct s_commands
{
	char			**argv;
	int				error;
	int				fd_in;
	int				fd_out;
	struct s_cmd	*next;
}	t_command;

typedef struct s_token
{
    t_token_type    type;
    char            *content;
    int             flag;
	int				exp;
	int				backslash;
    struct s_token  *next;
    
}       t_token;

typedef struct s_cmd
{
    char			argv;
    int				error;
    int				fd_in;
    int				fd_out;
    int				index;
    struct s_cmd    *next;
}    t_cmd;

typedef struct s_msh
{
    int             cmd_len;
    char            *input;
	char			**envp;
	char			**path;
	struct s_env	*env;
	int				parse_error;
	int				cmd_len;
	int				state;
	struct s_cmd	*cmd;
	struct s_token	*tokens;
	struct s_env	*env;
}       t_msh;

typedef struct s_env
{
	char			*type;
	char			*content;
	struct s_env	*next;
}		t_env;

extern int	g_signal;

void init_msh(char **envp, t_msh *msh);
t_env	*enviroment_lst(char **envp);
void get_input(t_msh *msh);

/////////////TOKENIZADOR//////////////
t_token			*set_tokens(char *line, t_msh *msh);
static void		set_backslash_token(char *line, int *i, t_token **tokens, int flag);
void			set_word_token(char *line, int *i, t_token **tokens);
void    		set_greather_token(char *line, int *i, t_token **tokens);
void    		set_lower_token(char *line, int *i, t_token **tokens);
void    		set_pipe_token(char *line, int *i, t_token **tokens);
void    		set_quote_token(char *line, int *i, t_token **tokens, t_msh *msh);
void    		set_double_quote_token(char *line, int *i, t_token **tokens, t_msh *msh);

/////////////UTILS//////////////
void    classify_token(char *token);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strchr(const char *s, int c);
char	*my_strtok(char *str, const char *delim);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s1);
size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t	ft_strlen(const char *s);

/////////////NODES//////////////
void		add_node_back(t_token **lst, t_token *new);
t_token 	new_node(t_token **content, int type, int flag);
void		create_token_lst(t_token **tok, int type, char *content, int flag);

/////////////ENVIRONMENT//////////////
t_env	*env_lst(char **envp);
static void init_env_lst(t_env **lst, char  **envp);

////////////////FLAGS/////////////////
void expand_flag(t_token *tok);

////////////////EXPAND/////////////////
char	*no_expand_var(char *s1, int *i);
void	expand_content(t_token *tok, t_msh *msh);
void	expand_tokens(t_token **tokens, t_msh *msh);
/* Executor */
void	executor(t_msh *msh);

/* Utils */
char	**get_path(void);
void	set_cmd_ind(t_cmd *cmd);


#endif
#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

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

typedef struct s_msh
{
    struct s_token  *tokens;
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

#endif
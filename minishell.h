#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

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

typedef struct s_token
{
    t_token_type    type;
    char            *content;
    int             flag;
    struct s_token  *next;
    
}       t_token;

typedef struct s_msh
{
    struct s_token  *tokens;
    char            *line;
    int             parse_error;
}       t_msh;

#endif
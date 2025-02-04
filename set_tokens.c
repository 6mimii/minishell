/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 19:11:58 by mohamed-dou       #+#    #+#             */
/*   Updated: 2025/02/04 15:14:15 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void set_backslash_token(char *line, int *i, t_token **tokens, int flag)
{
    int start;
    
    start = *i + 1;
    
    if (line[*i] == '\\' && line[*i + 1] != '\0')
    {
        if (line[*i + 1] == '$' || line[*i + 1] == '~') // si es esto, se ajusta el indice de inicio
            start--;
        if (line[*i + 1] && line[*i + 1] == '~') // si es esto, se avanza
            *i += 1;
        create_token_lst(tokens, T_WORD, ft_substr (line, start, (*i + 1 - start)), flag);   
    }
    else
        create_token_lst(tokens, T_WORD, ft_subdup(""), 2); // crea un token vacio
        *i += 1;
}

void    set_word_token(char *line, int *i, t_token **tokens)
{
    int start;
    int flag;

    start = *i;
    flag = (*i > 0 && line[*i - 1 != ' ' || (*i > 1 && line[*i - 2] == '\\')]); // automatiza a 1 o 0

    if (line[*i] == '\\')
    {
        set_backslash_token(line, i, tokens, flag);
    }
    else
    {
        while (line[*i] && line[*i] > 32 && line[*i] < 127                 // avanza mientras sea una palabra
            && line[*i] != '<' && line[*i] != '>' && line[*i] != '|'
            && line[*i] != '\'' && line[*i] != '\"' && line[*i] != '\\')
        {
            (*i)++;
        }
        create_token_lst(tokens, T_WORD, ft_substr(line, start, *i - start), flag); // ??
    }
}

int     check_pipes(t_msh *msh, t_token *token, int *flag)
{
    if (token->type == T_PIPE)
    {
        if (!*flag)                           
            return (error_msh(UNEXPECTED_TOK, msh, 2), 0); // sin comando previo
        if (!token->next)
            return (error_msh(UNEXPECTED_EOF, msh, 2), 0); // sin comando posterior
        if (token->next->type == T_PIPE)
            return (error_msh(UNEXPECTED_TOK, msh, 2), 0); // 2 pipes seguidas
    }
    *flag += 1;                                            // pipe en orden == 1
    return (1);
}

int     check_tokens(t_token **tokens, t_msh *msh, int flag)
{
    t_token *aux;

    aux = *tokens;
    while(aux)
    {
        if (check_pipes(msh, aux, &flag) == 0)
            return (0);
        if (aux->type != T_WORD && aux->type != T_Q && aux->type != T_DQ && aux->type != T_PIPE)
        {
            if (!aux->next)
                return (error_msh(UNEXPECTED_EOF, msh, 2), 0); // es un < << > >> sin sucesor
            if (aux->next->type != T_WORD && aux->next->type != T_Q && aux->next->type != T_DQ)
                return (error_msh(UNEXPECTED_TOK, msh, 127), 0); // token inesperado
        }
        else if (aux->type == T_WORD && !aux->next && aux->flag == 2) // palabra en ultima posicion
            return (error_msh(UNEXPECTED_EOF, msh, 2), 0);
        aux = aux->next;
    }    
}

t_token *set_tokens(char *line, t_msh *msh)
{
    int         i;
    t_token     *tokens;

    i = 0;
    tokens = NULL;
    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '<' && line[i] != '>'
                && line[i] != '|' && line[i] != '\n' && line[i] != '\''
                && line[i] != '\"')
                set_word_token(line, &i, &tokens);
        else if (line[i] == '<')
                set_lower_token(line, &i, &tokens);
        else if (line[i] == '>')
                set_greather_token(line, &i, &tokens);
        else if (line[i] == '|')
                set_pipe_token(line, &i, &tokens);
        else if (line[i] == '\'')
                set_quote_token(line, &i, &tokens, msh);
        else if (line[i] == '\"')
                set_doble_quote_token(line, &i, &tokens);
        else if (line[i] == ' ' || line[i] == '\n')
                i++;
    } 
    return (tokens);
}
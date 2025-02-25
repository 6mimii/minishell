/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 19:11:58 by mohamed-dou       #+#    #+#             */
/*   Updated: 2025/02/10 17:39:41 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void set_backslash_token(char *input, int *i, t_token **tokens, int flag)
{
    int start;
    
    start = *i + 1;
    
    if (input[*i] == '\\' && input[*i + 1] != '\0')
    {
        if (input[*i + 1] == '$' || input[*i + 1] == '~') // si es esto, se ajusta el indice de inicio
            start--;
        if (input[*i + 1] && input[*i + 1] == '~') // si es esto, se avanza
            *i += 1;
        create_token_lst(tokens, T_WORD, ft_substr (input, start, (*i + 1 - start)), flag);   
    }
    else
        create_token_lst(tokens, T_WORD, ft_subdup(""), 2); // crea un token vacio
        *i += 1;
}

void    set_word_token(char *input, int *i, t_token **tokens)
{
    int start;
    int flag;

    start = *i;
    flag = (*i > 0 && input[*i - 1 != ' ' || (*i > 1 && input[*i - 2] == '\\')]); // automatiza a 1 o 0

    if (input[*i] == '\\')
    {
        set_backslash_token(input, i, tokens, flag);
    }
    else
    {
        while (input[*i] && input[*i] > 32 && input[*i] < 127                 // avanza mientras sea una palabra
            && input[*i] != '<' && input[*i] != '>' && input[*i] != '|'
            && input[*i] != '\'' && input[*i] != '\"' && input[*i] != '\\')
        {
            (*i)++;
        }
        create_token_lst(tokens, T_WORD, ft_substr(input, start, *i - start), flag); // ??
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

t_token *set_tokens(char *input, t_msh *msh)
{
    int         i;
    t_token     *tokens;

    i = 0;
    tokens = NULL;
    while (input[i])
    {
        if (input[i] != ' ' && input[i] != '<' && input[i] != '>'
                && input[i] != '|' && input[i] != '\n' && input[i] != '\''
                && input[i] != '\"')
                set_word_token(input, &i, &tokens);
        else if (input[i] == '<')
                set_lower_token(input, &i, &tokens);
        else if (input[i] == '>')
                set_greather_token(input, &i, &tokens);
        else if (input[i] == '|')
                set_pipe_token(input, &i, &tokens);
        else if (input[i] == '\'')
                set_quote_token(input, &i, &tokens, msh);
        else if (input[i] == '\"')
                set_doble_quote_token(input, &i, &tokens);
        else if (input[i] == ' ' || input[i] == '\n')
                i++;
    } 
    return (tokens);
}
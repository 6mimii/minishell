/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 19:11:58 by mohamed-dou       #+#    #+#             */
/*   Updated: 2025/05/25 22:57:54 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_backslash_token(char *input, int *i, t_token **tokens, int flag)
{
	int	start;

	start = *i + 1;
	if (input[*i] == '\\' && input[*i + 1] != '\0')
	{
		if (input[*i + 1] == '$' || input[*i + 1] == '~')
			start--;
		if (input[*i + 1] && input[*i + 1] == '~')
			*i += 1;
		create_token_lst(tokens, T_WORD, ft_substr(input, start, (*i + 1
					- start)), flag);
	}
	else
		create_token_lst(tokens, T_WORD, ft_strdup(""), 2);
	*i += 1;
}

void set_logical_operator_token(char *line, int *i, t_token **tokens)
{
    if (line[*i] && line[*i + 1] && line[*i] == '&' && line[*i + 1] == '&')
    {
        char *str = ft_strdup("&&");
        create_token_lst(tokens, T_WORD, str, 0);
        *i += 2;
    }
}

void	set_word_token(char *input, int *i, t_token **tokens)
{
	int	start;
	int	flag;

	if (input[*i] == '&' && input[*i + 1] == '&')
	{
		set_logical_operator_token(input, i, tokens);
		return;
	}

	start = *i;
	flag = ((*i > 0 && input[*i - 1] != ' ') || (*i > 1 && input[*i - 2] == '\\'));
	if (input[*i] == '\\')
	{
		set_backslash_token(input, i, tokens, flag);
	}
	else
	{
		while (input[*i] && input[*i] > 32 && input[*i] < 127
			&& input[*i] != '<' && input[*i] != '>' && input[*i] != '|'
			&& input[*i] != '\'' && input[*i] != '\"' && input[*i] != '\\'
			&& input[*i] != ' ' && input[*i] != '\n'
			&& !(input[*i] == '&' && input[*i + 1] == '&'))
		{
			(*i)++;
		}
		create_token_lst(tokens, T_WORD, ft_substr(input, start, *i - start),
			flag);
	}
}

int	check_pipes(t_msh *msh, t_token *token, int *flag)
{
	if (token->type == T_PIPE)
	{
		if (!*flag)
			return (error_msh(UNEXPECTED_TOK, msh, 2), 0); 
		if (!token->next)
			return (error_msh(UNEXPECTED_EOF, msh, 2), 0);
		if (token->next->type == T_PIPE)
			return (error_msh(UNEXPECTED_TOK, msh, 2), 0);
	}
	*flag += 1;
	return (1);
}

int	check_tokens(t_token **tokens, t_msh *msh, int flag)
{
	t_token	*aux;

	aux = *tokens;
	while (aux)
	{
		if (check_pipes(msh, aux, &flag) == 0)
			return (0);
		else if (aux->type != T_WORD && aux->type != T_Q && aux->type != T_DQ
			&& aux->type != T_PIPE)
		{
			if (!aux->next)
				return (error_msh(UNEXPECTED_EOF, msh, 2), 0);
			if (aux->next->type != T_WORD && aux->next->type != T_Q
				&& aux->next->type != T_DQ)
				return (error_msh(UNEXPECTED_TOK, msh, 127), 0);
		}
		else if (aux->type == T_WORD)
		{
			if (!aux->next && aux->flag == 2)
				return (error_msh(UNEXPECTED_EOF, msh, 2), 0);
		}
		aux = aux->next;
	}
	return (1);
}

t_token	*set_tokens(char *input, t_msh *msh)
{
	int		i;
	t_token	*tokens;
	int     in_quotes;

	i = 0;
	tokens = NULL;
	in_quotes = 0;
	
	while (input[i] && !msh->parse_error)
	{
		if (input[i] == '\'' && in_quotes != 2)
		{
			if (in_quotes == 1)
				in_quotes = 0;
			else
				in_quotes = 1;
			set_quote_token(input, &i, &tokens, msh);
		}
		else if (input[i] == '\"' && in_quotes != 1)
		{
			if (in_quotes == 2)
				in_quotes = 0;
			else
				in_quotes = 2;
			set_double_quote_token(input, &i, &tokens, msh);
		}
		else if (in_quotes == 0 && input[i] == '&' && input[i + 1] == '&')
			set_logical_operator_token(input, &i, &tokens);
		else if (in_quotes == 0 && input[i] == '<')
			set_lower_token(input, &i, &tokens);
		else if (in_quotes == 0 && input[i] == '>')
			set_greather_token(input, &i, &tokens);
		else if (in_quotes == 0 && input[i] == '|')
			set_pipe_token(input, &i, &tokens);
		else if (input[i] != ' ' && input[i] != '\n')
			set_word_token(input, &i, &tokens);
		else
			i++;
	}
	
	if (in_quotes != 0)
	{
		if (in_quotes == 1)
			error_msh(WRONG_Q, msh, 2);
		else
			error_msh(WRONG_DQ, msh, 2);
		msh->parse_error = 1;
	}
	
	return (tokens);
}

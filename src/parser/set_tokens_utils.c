/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:50:00 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:21:53 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	handle_quotes(char *input, int *i, t_token **tokens, void *data)
{
	t_msh	*msh;
	int		*in_quotes;

	msh = ((void **)data)[0];
	in_quotes = ((void **)data)[1];
	(void)msh;
	if (input[*i] == '\'' && *in_quotes != 2)
	{
		if (*in_quotes == 1)
			*in_quotes = 0;
		else
			*in_quotes = 1;
		set_quote_token(input, i, tokens);
	}
	else if (input[*i] == '"' && *in_quotes != 1)
	{
		if (*in_quotes == 2)
			*in_quotes = 0;
		else
			*in_quotes = 2;
		set_double_quote_token(input, i, tokens);
	}
}

static void	handle_special_tokens(char *input, int *i, t_token **tokens,
		int in_quotes)
{
	if (in_quotes == 0 && input[*i] == '&' && input[*i + 1] == '&')
		set_logical_operator_token(input, i, tokens);
	else if (in_quotes == 0 && input[*i] == '<')
		set_lower_token(input, i, tokens);
	else if (in_quotes == 0 && input[*i] == '>')
		set_greather_token(input, i, tokens);
	else if (in_quotes == 0 && input[*i] == '|')
		set_pipe_token(input, i, tokens);
	else if (input[*i] != ' ' && input[*i] != '\n')
		set_word_token(input, i, tokens);
	else
		(*i)++;
}

t_token	*set_tokens(char *input, t_msh *msh)
{
	int		i;
	t_token	*tokens;
	int		in_quotes;
	void	*data[2];

	i = 0;
	tokens = NULL;
	in_quotes = 0;
	data[0] = msh;
	data[1] = &in_quotes;
	while (input[i] && !msh->parse_error)
	{
		if ((input[i] == '\'' && in_quotes != 2) || (input[i] == '"'
				&& in_quotes != 1))
			handle_quotes(input, &i, &tokens, data);
		else
			handle_special_tokens(input, &i, &tokens, in_quotes);
	}
	return (tokens);
}

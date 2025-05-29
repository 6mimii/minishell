/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 19:11:58 by mohamed-dou       #+#    #+#             */
/*   Updated: 2025/05/29 15:42:25 by mdoudi-b         ###   ########.fr       */
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

void	set_logical_operator_token(char *line, int *i, t_token **tokens)
{
	char	*str;

	if (line[*i] && line[*i + 1] && line[*i] == '&' && line[*i + 1] == '&')
	{
		str = ft_strdup("&&");
		create_token_lst(tokens, T_AND, str, 0);
		*i += 2;
	}
	else if (line[*i] && line[*i + 1] && line[*i] == '|' && line[*i + 1] == '|')
	{
		str = ft_strdup("||");
		create_token_lst(tokens, T_OR, str, 0);
		*i += 2;
	}
}

static int	calculate_flag(char *input, int i)
{
	return ((i > 0 && input[i - 1] != ' ') || (i > 1 && input[i - 2] == '\\'));
}

static void	process_word_chars(char *input, int *i)
{
	while (input[*i] && input[*i] > 32 && input[*i] < 127 && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '|' && input[*i] != '\''
		&& input[*i] != '\"' && input[*i] != '\\' && input[*i] != ' '
		&& input[*i] != '\n' && !(input[*i] == '&' && input[*i + 1] == '&')
		&& !(input[*i] == '|' && input[*i + 1] == '|'))
	{
		(*i)++;
	}
}

void	set_word_token(char *input, int *i, t_token **tokens)
{
	int	start;
	int	flag;

	if ((input[*i] == '&' && input[*i + 1] == '&') || (input[*i] == '|'
			&& input[*i + 1] == '|'))
	{
		set_logical_operator_token(input, i, tokens);
		return ;
	}
	start = *i;
	flag = calculate_flag(input, *i);
	if (input[*i] == '\\')
		set_backslash_token(input, i, tokens, flag);
	else
	{
		process_word_chars(input, i);
		create_token_lst(tokens, T_WORD, ft_substr(input, start, *i - start),
			flag);
	}
}

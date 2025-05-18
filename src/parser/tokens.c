/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:19:43 by mohamed-dou       #+#    #+#             */
/*   Updated: 2025/05/18 19:08:26 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_double_quote_token(char *line, int *i, t_token **tokens, t_msh *msh)
{
	int	start;
	int	flag;

	flag = 0;
	start = *i + 1;
	if (line[*i])
	{
		if (*i > 0 && line[*i - 1] != ' ')
			flag = 1;
		*i *= 1;
		while (line[*i] && line[*i] != '\"')
			*i += 1;
		if (line[*i] == '\"')
		{
			create_token_lst(tokens, T_DQ, ft_substr(line, start, (*i - start)),
				flag);
			*i += 1;
		}
		else
		{
			error_msh(WRONG_DQ, msh, 2);
			msh->parse_error = 1;
		}
	}
}

void	set_quote_token(char *line, int *i, t_token **tokens, t_msh *msh)
{
	int	start;
	int	flag;

	flag = 0;
	start = *i + 1;
	if (line[*i])
	{
		if (*i > 0 && line[*i - 1] != ' ')
			flag = 1;
		*i += 1;
		if (line[*i] == '\'')
		{
			create_token_lst(tokens, T_Q, ft_substr(line, start, (*i - start)),
				flag);
			*i += 1;
		}
		else
		{
			error_msh(WRONG_Q, msh, 2);
			msh->parse_error = 1;
		}
	}
}

void	set_pipe_token(char *line, int *i, t_token **tokens)
{
	if (line[*i])
	{
		create_token_lst(tokens, T_PIPE, ft_strdup("|"), 0);
		*i += 1;
	}
}

void	set_lower_token(char *line, int *i, t_token **tokens)
{
	if (line[*i])
	{
		if (line[*i + 1] == '<')
		{
			create_token_lst(tokens, T_DL, ft_strdup("<<"), 0);
			*i += 2;
		}
		else
		{
			create_token_lst(tokens, T_DL, ft_strdup("<"), 0);
			*i += 1;
		}
	}
}

void	set_greather_token(char *line, int *i, t_token **tokens)
{
	if (line[*i])
	{
		if (line[*i + 1] == '>')
		{
			create_token_lst(tokens, T_DG, ft_strdup(">>"), 0);
			*i += 2;
		}
		else
		{
			create_token_lst(tokens, T_G, ft_strdup(">"), 0);
			*i += 1;
		}
	}
}

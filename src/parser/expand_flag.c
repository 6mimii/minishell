/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:04:25 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/20 01:27:04 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	expand_flag(t_token *tok)
{
	t_token	*aux;
	int     token_count = 0;

	write(2, "In expand_flag function\n", 24);

	if (!tok) {
		write(2, "Token list is NULL\n", 19);
		return;
	}

	aux = tok;
	while (aux)
	{
		token_count++;
		
		if (!aux->content) {
			write(2, "Warning: Token with NULL content\n", 33);
			aux = aux->next;
			continue;
		}
		
		// Debugging: Log the token content and type before processing
		write(2, "Debug: Token before processing: ", 31);
		write(2, aux->content, ft_strlen(aux->content));
		write(2, " Type: ", 7);
		char type[12];
		sprintf(type, "%d", aux->type);
		write(2, type, ft_strlen(type));
		write(2, "\n", 1);
		
		aux->exp = 0;
		if (aux->type == T_WORD || aux->type == T_DQ)
		{
			if (check_dollar(aux->content) == 1)
				aux->exp = 1;
			else if (check_home(aux->content) == 1)
				aux->exp = 2;
			else if (check_home(aux->content) == 2)
				aux->exp = 3;
			}
		
		// Debugging: Log the token content and exp flag after processing
		write(2, "Debug: Token after processing: ", 30);
		write(2, aux->content, ft_strlen(aux->content));
		write(2, " Exp: ", 6);
		char exp[12];
		sprintf(exp, "%d", aux->exp);
		write(2, exp, ft_strlen(exp));
		write(2, "\n", 1);
		
		aux = aux->next;
	}
	
	char num[12];
	write(2, "Processed tokens in expand_flag: ", 33);
	sprintf(num, "%d", token_count);
	write(2, num, ft_strlen(num));
	write(2, "\n", 1);
}

int	check_home(const char *str)
{
	if (!str || !*str) 
		return (0);
		
	if (str[0] == '~' && (!str[1] || str[1] == '/'))
		return (1);
	else if (str[0] == '~' && str[1] == '/')
		return (2);
	return (0);
}

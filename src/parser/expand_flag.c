/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:04:25 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/21 00:49:04 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	expand_flag(t_token *tok)
{
	t_token	*aux;

	if (!tok)
		return;

	aux = tok;
	while (aux)
	{
		if (!aux->content) {
			aux = aux->next;
			continue;
		}
		
		aux->exp = 0;
		// Importante: solo expandir en T_WORD o T_DQ (comillas dobles)
		// T_Q (comillas simples) nunca debe expandirse
		if (aux->type == T_WORD || aux->type == T_DQ)
		{
			if (check_dollar(aux->content) == 1)
				aux->exp = 1;
			else if (check_home(aux->content) == 1)
				aux->exp = 2;
			else if (check_home(aux->content) == 2)
				aux->exp = 3;
		}
		// Asegurar que los tokens de tipo T_Q (comillas simples) nunca se expandan
		else if (aux->type == T_Q) 
		{
			aux->exp = 0;  // Forzar a 0 para evitar cualquier expansión
		}
		
		aux = aux->next;
	}
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

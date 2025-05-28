/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:30:26 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/28 20:11:36 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	command_content(t_cmd *new, t_token *tok)
{
	t_token	*aux;
	int		i;

	aux = tok;
	i = 0;
	while (aux && aux->type != T_PIPE)
	{
		if (is_logical_operator(aux))
		{
			aux = aux->next;
			continue;
		}
		if (aux->type == T_G || aux->type == T_DG || aux->type == T_L
			|| aux->type == T_DL)
			aux = aux->next;
		else if (!process_command_token(new, aux, &i))
			return (0);
		aux = aux ? aux->next : NULL;
	}
	new->argv[i] = NULL;
	return (1);
}

int	command_len(t_token *tok)
{
	int		i;
	t_token	*aux;

	aux = tok;
	i = 0;
	while (aux && aux->type != T_PIPE)
	{
		if (is_logical_operator(aux))
		{
			aux = aux->next;
			continue;
		}
		if (aux->type == T_G || aux->type == T_DG || aux->type == T_L
			|| aux->type == T_DL)
			aux = aux->next;
		else
			i++;
		aux = aux ? aux->next : NULL;
	}
	return (i);
}







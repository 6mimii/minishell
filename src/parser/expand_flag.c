/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:04:25 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/01 17:29:52 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void expand_flag(t_token *tok)
{
	t_token *aux;

	aux = tok;
	while (aux)
	{
		aux->exp = 0;
		if (aux->type == T_WORD || aux->type == T_DQ)
		{
			// if (check_dollar(aux->content) == 1)	
			// 	aux->exp = 1;
			// else if (check_home(aux->content) == 1)
			// 	aux->exp = 2;
			// else if (check_home(aux->content) == 2)
			// 	aux->exp = 3;
		}
		aux = aux->next;
	}
}
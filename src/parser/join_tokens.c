/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:00:11 by mimi-notebo       #+#    #+#             */
/*   Updated: 2025/05/27 16:40:14 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*pop(t_token *tok)
{
	t_token	*aux;

	if (tok->next)
		aux = tok->next;
	else
		aux = NULL;
	free(tok->content);
	free(tok);
	return (aux);
}

static t_token	*join_pop(t_token *tok)
{
	t_token	*aux;
	char	*first;
	char	*next;

	aux = tok;
	first = ft_strdup(tok->content);
	next = ft_strdup(tok->next->content);
	free(aux->content);
	aux->content = ft_strjoin(first, next);
	free(first);
	free(next);
	aux->next = pop(tok->next);
	return (aux);
}

void	join_tokens(t_token **tokens)
{
	t_token			*aux;
	t_token_type	original_type;

	aux = *tokens;
	while (aux)
	{
		while ((aux->type == T_WORD || aux->type == T_Q || aux->type == T_DQ)
			&& aux->next && aux->next->flag == 1)
		{
			original_type = aux->type;
			aux = join_pop(aux);
			if (original_type == T_Q || original_type == T_DQ)
				aux->type = original_type;
		}
		aux = aux->next;
	}
}

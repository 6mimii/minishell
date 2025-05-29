/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 15:21:28 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:12:46 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_node_back(t_token **lst, t_token *new)
{
	t_token	*new_node;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	new_node = *lst;
	while (new_node->next != NULL)
		new_node = new_node->next;
	new_node->next = new;
}

t_token	*new_node(char *content, int type, int flag)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
	{
		free(content);
		return (NULL);
	}
	new->content = content;
	new->type = type;
	new->flag = flag;
	new->next = NULL;
	new->exp = 0;
	new->backslash = 0;
	return (new);
}

void	create_token_lst(t_token **tok, int type, char *content, int flag)
{
	t_token	*aux;

	if (!content)
	{
		write(2, "Warning: Trying to create token with NULL content\n", 49);
		return ;
	}
	aux = new_node(content, type, flag);
	if (!aux)
	{
		write(2, "Failed to create token node\n", 28);
		free(content);
		return ;
	}
	if (!*tok)
		*tok = aux;
	else
		add_node_back(tok, aux);
}

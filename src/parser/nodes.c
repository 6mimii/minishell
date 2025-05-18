/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 15:21:28 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/18 19:16:19 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_node_back(t_token **lst, t_token *new)
{
	t_token		*new_node;

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
		return (NULL);
	new->content = ft_strdup(content);
	new->type = type;
	new->flag = flag;
	return (new);
}

void	create_token_lst(t_token **tok, int type, char *content, int flag)
{
	t_token	*aux;

	aux = new_node(content, type, flag);
	if (!*tok)
		*tok = new_node(content, type, flag);
	else
	{
		aux = new_node(content, type, flag);
		add_node_back(tok, aux);
	}
}

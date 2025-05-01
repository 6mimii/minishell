/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:20:37 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/01 17:24:22 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_back_command (t_cmd **lst, t_cmd *new)
{
	t_cmd	*new_node;
	
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

t_cmd	*new_node_command(void)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->error = 0;
	new->fd_in = 0;
	new->fd_out = 0;
	new->next = NULL;
	return (new);
}

void	create_command_list(t_cmd **cmd, t_cmd *new)
{
	if (!*cmd)
		*cmd = new;
	else
		add_back_command(cmd, new);
}

int	command_content(t_cmd *new, t_token *tok)
{
	t_token	*aux;
	int		i;
	
	aux = tok;
	i = 0;
	
	while(aux && aux->type != T_PIPE)
	{
		if (aux->type == T_G || aux->type == T_DG || aux ->type == T_L || aux->type == T_DL)
			aux = aux->next;
		else
		{
			if (aux->content)
				new->argv[i] = ft_strdup(aux->content);
			else
				new->argv[i] = ft_strdup("");
			if (!new->argv[i])
				return(0);
		}
		aux = aux->next;
	}
	new->argv[i] = NULL;
	return(1);
}

int command_len(t_token *tok)
{
	int		i;
	t_token	*aux;

	aux = tok;
	i = 0;
	while (aux && aux->type != T_PIPE)
	{
		if (aux->type == T_G || aux->type == T_DG
			|| aux ->type == T_L || aux->type == T_DL)
			aux = aux->next;
		else
			i++;
		aux = aux->next;
	}
	return (i);
}
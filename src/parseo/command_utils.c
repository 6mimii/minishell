/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:20:37 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/03/17 16:40:22 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_back_command (t_command **lst, t_command *new)
{
	t_command	*new_node;
	
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

t_command	*new_node_cmd(void)
{
	t_command	*new;

	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->error = 0;
	new->fd_in = 0;
	new->fd_out = 0;
	new->next = NULL;
	return (new);
}

void	create_command_list(t_command **cmd, t_command *new)
{
	if (!*cmd)
		*cmd = new;
	else
		add_back_command(cmd, new);
}

int	command_content(t_command *new, t_token *tok)
{
	t_token	*aux;
	int		i;
	
	aux = tok;
	i = 0;
	
	while(aux && aux->type != T_PIPE)
	{
		if (aux->type == T_G || aux->type == T_DG || aux ->type == T_L || aux->type = T_DL)
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
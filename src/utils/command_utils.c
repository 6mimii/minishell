/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:20:37 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/27 17:31:04 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_back_command(t_cmd **lst, t_cmd *new)
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
	new->fd_in = STDIN_FILENO;
	new->fd_out = STDOUT_FILENO;
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



int	process_command_token(t_cmd *new, t_token *aux, int *i)
{
	if (aux->content)
		new->argv[*i] = ft_strdup(aux->content);
	else
		new->argv[*i] = ft_strdup("");
	if (!new->argv[*i])
		return (0);
	(*i)++;
	return (1);
}


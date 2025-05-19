/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:20:37 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/20 01:27:04 by mimi-notebo      ###   ########.fr       */
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

static int is_logical_operator(t_token *token)
{
    if (!token || !token->content)
        return 0;
    
    if (ft_strcmp(token->content, "&&") == 0)
        return 1;
    
    return 0;
}

int	command_content(t_cmd *new, t_token *tok)
{
	t_token	*aux;
	int		i;

	aux = tok;
	i = 0;
	while (aux && aux->type != T_PIPE && !is_logical_operator(aux))
	{
		if (aux->type == T_G || aux->type == T_DG || aux->type == T_L
			|| aux->type == T_DL)
			aux = aux->next;
		else
		{
			if (aux->content)
				new->argv[i] = ft_strdup(aux->content);
			else
				new->argv[i] = ft_strdup("");
			if (!new->argv[i])
				return (0);
			i++;
		}
		if (aux)
			aux = aux->next;
		else
			break;
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
	while (aux && aux->type != T_PIPE && !is_logical_operator(aux))
	{
		if (aux->type == T_G || aux->type == T_DG || aux->type == T_L
			|| aux->type == T_DL)
			aux = aux->next;
		else
			i++;
		if (aux)
			aux = aux->next;
		else
			break;
	}
	return (i);
}
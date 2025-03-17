/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:39:35 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/03/17 12:47:16 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_command_len(t_token *token)
{
	t_token *aux;
	int		len;
	
	aux = token;
	len = 0;
	while(aux && aux->type != T_PIPE)
	{
		if (aux->type == T_G || aux->type == T_DG || aux->type == T_L || aux->type == T_DL)
			aux = aux->next;
		else
			len++;
		aux = aux->next;
	}
	return(len);
}

static void	set_command(t_msh *msh, t_token **tokens)
{
	t_command 	*new_command;
	int			len;

	len = get_command_len(*tokens);
	new_command = new_node_command();
	new_command->argv = (char **)malloc(sizeof(char *) * (len + 1));
	if (!new_command->argv)
		return ;
	if (len > 0)
	{
		if (command_content(new_command, *tokens) == 0)
		{
			error_msh
		}
	}
}

void	get_command(t_msh *msh)
{
	t_token	*tmp;

	tmp = msh->tokens;
	while (tmp)
	{
		if (tmp->type != T_PIPE)
			set_cmd(msh, &tmp);
		else
			tmp = tmp->next;
	}
}
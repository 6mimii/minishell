/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:28:43 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:28:51 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_cmd	*create_new_command(int len)
{
	t_cmd	*new_command;

	new_command = new_node_command();
	if (!new_command)
		return (NULL);
	new_command->argv = (char **)malloc(sizeof(char *) * (len + 1));
	if (!new_command->argv)
	{
		free(new_command);
		return (NULL);
	}
	return (new_command);
}

static void	populate_command_content(t_cmd *new_command, t_token *tokens,
		t_msh *msh, int len)
{
	if (len > 0)
	{
		if (command_content(new_command, tokens) == 0)
		{
			error_msh(MLLC_ERR, msh, 2);
			new_command->error = 1;
		}
	}
	else
	{
		new_command->argv[0] = NULL;
	}
}

static void	set_cmd(t_msh *msh, t_token **tokens)
{
	t_cmd	*new_command;
	int		len;

	if (!tokens || !*tokens)
		return ;
	len = get_command_len(*tokens);
	new_command = create_new_command(len);
	if (!new_command)
		return ;
	populate_command_content(new_command, *tokens, msh, len);
	finalize_command(msh, new_command, tokens);
}

void	get_command(t_msh *msh)
{
	t_token	*tmp;

	tmp = msh->tokens;
	while (tmp)
	{
		if (tmp->type != T_PIPE && !is_logical_operator(tmp))
		{
			set_cmd(msh, &tmp);
		}
		else if (is_logical_operator(tmp))
		{
			tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
		}
		else
		{
			tmp = tmp->next;
		}
	}
}

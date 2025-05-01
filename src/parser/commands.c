/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:39:35 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/01 15:29:26 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_fd(t_token **tok, t_cmd *new, t_msh *msh)
{
	while (*tok && (*tok)->type != T_PIPE)
	{
		if ((*tok)->type == T_G)
			set_outfile(tok, new, msh);
		else if ((*tok)->type == T_DG)
			set_append(tok, new, msh);
		else if ((*tok)->type == T_L)
			set_infile(tok, new, msh);
		else if ((*tok)->type == T_DL)
			set_heredoc(tok, new, msh);
		else if ((*tok)->type == T_WORD)
			*tok = (*tok)->next;
	}
}

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

static void	set_cmd(t_msh *msh, t_token **tokens)
{
	t_cmd 		*new_command;
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
			error_msh(MLLC_ERR, msh, 2);
			new_command->error = 1;
		}
	}
	else
		new_command->argv[0] = NULL;
	set_fd(tokens, new_command, msh);
	msh->cmd_len += 1;
	create_cmd_lst(&msh->cmd, new_command);
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
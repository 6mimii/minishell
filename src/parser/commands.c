/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:56:13 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:38:48 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_logical_operator(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == T_AND || token->type == T_OR)
		return (1);
	return (0);
}

static void	process_token(t_token **tok, t_cmd *new, t_msh *msh)
{
	if ((*tok)->type == T_G)
		set_outfile(tok, new, msh);
	else if ((*tok)->type == T_DG)
		set_append(tok, new, msh);
	else if ((*tok)->type == T_L)
		set_infile(tok, new, msh);
	else if ((*tok)->type == T_DL)
		set_heredoc(tok, new, msh);
	else if ((*tok)->type == T_WORD || *tok)
		*tok = (*tok)->next;
}

static void	set_fd(t_token **tok, t_cmd *new, t_msh *msh)
{
	while (*tok && (*tok)->type != T_PIPE && !is_logical_operator(*tok))
	{
		process_token(tok, new, msh);
		if (new->error)
			break ;
	}
}

int	get_command_len(t_token *token)
{
	t_token	*aux;
	int		len;

	aux = token;
	len = 0;
	while (aux && aux->type != T_PIPE && !is_logical_operator(aux))
	{
		if (aux->type == T_G || aux->type == T_DG || aux->type == T_L
			|| aux->type == T_DL)
		{
			if (aux->next)
				aux = aux->next;
			else
				break ;
		}
		else
		{
			len++;
		}
		if (aux)
			aux = aux->next;
		else
			break ;
	}
	return (len);
}

void	finalize_command(t_msh *msh, t_cmd *new_command, t_token **tokens)
{
	set_fd(tokens, new_command, msh);
	msh->cmd_len += 1;
	create_command_list(&msh->cmd, new_command);
	while (*tokens && (*tokens)->type != T_PIPE
		&& !is_logical_operator(*tokens))
	{
		*tokens = (*tokens)->next;
	}
}

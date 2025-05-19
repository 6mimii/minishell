/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:39:35 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/20 01:27:04 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to check if a token is a logical operator (like &&)
static int is_logical_operator(t_token *token)
{
    if (!token || !token->content)
        return 0;
    
    // Check for logical operators
    if (ft_strcmp(token->content, "&&") == 0)
        return 1;
    
    return 0;
}

static void	set_fd(t_token **tok, t_cmd *new, t_msh *msh)
{
	while (*tok && (*tok)->type != T_PIPE && !is_logical_operator(*tok))
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
		else if (*tok)
			*tok = (*tok)->next;
		
		if (new->error)
			break;
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
				aux = aux->next; // Saltar el token de redirección y su argumento
			else
				break;
		}
		else
		{
			len++; // Contar solo tokens de palabras (comandos/argumentos)
		}
		if (aux)
			aux = aux->next;
		else
			break;
	}
	return (len);
}

static void	set_cmd(t_msh *msh, t_token **tokens)
{
	t_cmd	*new_command;
	int		len;

	if (!tokens || !*tokens)
		return;

	len = get_command_len(*tokens);
	new_command = new_node_command();
	if (!new_command)
		return;
		
	new_command->argv = (char **)malloc(sizeof(char *) * (len + 1));
	if (!new_command->argv)
	{
		free(new_command);
		return;
	}
	
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
	create_command_list(&msh->cmd, new_command);
	
	// Skip tokens until we reach a pipe or logical operator or the end
	while (*tokens && (*tokens)->type != T_PIPE && !is_logical_operator(*tokens)) {
		*tokens = (*tokens)->next;
	}
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
		else
		{
			tmp = tmp->next;
		}
	}
}

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
			tmp = tmp->next; // Skip logical operator
		}
		else
		{
			tmp = tmp->next;
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:18 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/21 00:12:13 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*aux;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		aux = (*tokens)->next;
		if ((*tokens)->content)
			free((*tokens)->content);
		free(*tokens);
		*tokens = aux;
	}
	*tokens = NULL;
}

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		free(matrix[i++]);
	free(matrix);
	matrix = NULL;
}

void	free_commands(t_cmd **cmd)
{
	t_cmd	*aux;

	if (!cmd || !*cmd)
		return ;
	while (*cmd)
	{
		aux = (*cmd)->next;
		if ((*cmd)->fd_in > 2)
		{
			if (access(".here_doc.tmp", F_OK) == 0)
				unlink(".here_doc.tmp");
			close((*cmd)->fd_in);
		}
		if ((*cmd)->fd_out > 2)
			close((*cmd)->fd_out);
		free_matrix((*cmd)->argv);
		free(*cmd);
		*cmd = aux;
	}
	*cmd = NULL;
}

void	free_msh(t_msh *msh)
{
	int i;
	
	if (msh->tokens)
		free_tokens(&msh->tokens);
	if (msh->cmd)
		free_commands(&msh->cmd);
	if (msh->input)
	{
		free(msh->input);
		msh->input = NULL;
	}
	if (msh->path)
	{
		free_matrix(msh->path);
		msh->path = NULL;
	}
	
	// Liberar la lista de variables eliminadas
	if (msh->unset_vars)
	{
		i = 0;
		while (msh->unset_vars[i])
			free(msh->unset_vars[i++]);
		free(msh->unset_vars);
		msh->unset_vars = NULL;
	}
	
	msh->cmd_len = 0;
	msh->parse_error = 0;
}

void	free_env(t_env *env)
{
	t_env	*aux;

	if (!env)
		return ;
	while (env)
	{
		aux = env->next;
		free(env->type);
		free(env->content);
		free(env);
		env = aux;
	}
}

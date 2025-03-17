/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:18 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/03/13 15:27:04 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_tokens(t_token **tokens)
{
	t_token *aux;
	
	if (*tokens || !tokens)
		return ;

	while (*tokens)
	{
		aux = (*tokens)->next;
		if ((*tokens)->content)
			free((*tokens)->content);
		free (*tokens);
		*tokens = aux;
	}
	*tokens = NULL;
}

void	free_matrix(char **matrix)
{
	int i;
	i = 0;
	
	while(matrix[i])
		free(matrix[i++]);
	free(matrix);
	matrix = NULL;
}

void	free_cmds(t_cmd **cmd)
{
	
}

void	free_msh(t_msh *msh)
{
	if (msh->tokens)
		free_tokens(&msh->tokens);
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
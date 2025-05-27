/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:33:51 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/27 17:34:39 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_msh_resources(t_msh *msh)
{
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
}

static void	free_unset_vars(t_msh *msh)
{
	int	i;

	if (msh->unset_vars)
	{
		i = 0;
		while (msh->unset_vars[i])
			free(msh->unset_vars[i++]);
		free(msh->unset_vars);
		msh->unset_vars = NULL;
	}
}

void	free_msh(t_msh *msh)
{
	if (msh->tokens)
		free_tokens(&msh->tokens);
	if (msh->cmd)
		free_commands(&msh->cmd);
	free_msh_resources(msh);
	free_unset_vars(msh);
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

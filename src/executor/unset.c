/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:50:08 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/04/10 17:10:07 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env_node(t_env *env)
{
	free(env->type);
	free(env->content);
	free(env);
}

static void	delete_var(t_msh *msh, char *var)
{
	t_env	*curr;
	t_env	*prev;

	prev = msh->env;
	curr = msh->env->next;
	if (ft_strncmp(prev->type, var, ft_strlen(var) + 1) == 0)
	{
		free_env_node(prev);
		msh->env = curr;
		return ;
	}
	while (curr)
	{
		if (ft_strncmp(curr->type, var, ft_strlen(var) + 1) == 0)
		{
			prev->next = curr->next;
			free_env_node(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	ft_unset(t_msh *msh, t_cmd *cmd)
{
	int		i;

	i = 0;
	while (cmd->argv[++i])
		delete_var(msh, cmd->argv[i]);
	msh->state = 0;
}

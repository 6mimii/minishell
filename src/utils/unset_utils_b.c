/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils_b.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:45:59 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/27 17:46:51 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_node(t_env *env)
{
	if (!env)
		return ;
	if (env->type)
		free(env->type);
	if (env->content)
		free(env->content);
	free(env);
}

void	process_unset_args(t_msh *msh, t_cmd *cmd, int *changed)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
	{
		if (delete_env_var(msh, cmd->argv[i]))
		{
			*changed = 1;
			add_to_unset_list(msh, cmd->argv[i]);
		}
		i++;
	}
}

void	cleanup_envp_array(char **array, int count)
{
	while (--count >= 0)
		free(array[count]);
	free(array);
}

int	count_env_nodes(t_env *env)
{
	t_env	*curr;
	int		count;

	count = 0;
	curr = env;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

char	*create_env_string(t_env *curr)
{
	char	*tmp;
	char	*result;

	if (!curr->content)
		return (ft_strdup(curr->type));
	tmp = ft_strjoin(curr->type, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, curr->content);
	free(tmp);
	return (result);
}

int	delete_env_var(t_msh *msh, char *var)
{
	t_env	*curr;
	t_env	*prev;

	if (!msh->env || !var)
		return (0);
	prev = NULL;
	curr = msh->env;
	if (curr && ft_strcmp(curr->type, var) == 0)
	{
		msh->env = curr->next;
		free_env_node(curr);
		return (1);
	}
	while (curr)
	{
		if (ft_strcmp(curr->type, var) == 0)
		{
			prev->next = curr->next;
			free_env_node(curr);
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

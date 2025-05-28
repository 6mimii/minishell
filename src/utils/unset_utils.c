/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:43:54 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/28 20:11:36 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**create_envp_array(t_msh *msh)
{
	int		env_count;
	char	**new_envp;
	t_env	*curr;
	int		i;

	env_count = count_env_nodes(msh->env);
	new_envp = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	curr = msh->env;
	while (curr)
	{
		new_envp[i] = create_env_string(curr);
		if (!new_envp[i])
		{
			cleanup_envp_array(new_envp, i);
			return (NULL);
		}
		i++;
		curr = curr->next;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

int	get_unset_vars_length(t_msh *msh, char *var)
{
	int	i;

	if (!msh->unset_vars)
		return (0);
	i = 0;
	while (msh->unset_vars[i])
	{
		if (ft_strcmp(msh->unset_vars[i], var) == 0)
			return (-1);
		i++;
	}
	return (i);
}

char	**create_new_unset_list(t_msh *msh, char *var, int len)
{
	char	**new_list;
	int		i;

	new_list = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_list)
		return (NULL);
	for (i = 0; i < len; i++)
		new_list[i] = ft_strdup(msh->unset_vars[i]);
	new_list[len] = ft_strdup(var);
	new_list[len + 1] = NULL;
	return (new_list);
}



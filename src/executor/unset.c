/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:50:08 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/23 18:05:11 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_env_node(t_env *env)
{
	if (!env)
		return;
	if (env->type)
		free(env->type);
	if (env->content)
		free(env->content);
	free(env);
}

static int	count_env_nodes(t_env *env)
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

static int delete_env_var(t_msh *msh, char *var) {
    t_env *curr;
    t_env *prev;

    if (!msh->env || !var)
        return (0);

    prev = NULL;
    curr = msh->env;

    if (curr && ft_strcmp(curr->type, var) == 0) {
        msh->env = curr->next;
        free_env_node(curr);
        return (1);
    }

    while (curr) {
        if (ft_strcmp(curr->type, var) == 0) {
            if (prev)
                prev->next = curr->next;
            free_env_node(curr);
            return (1);
        }
        prev = curr;
        curr = curr->next;
    }
    return (0);
}

static char	*create_env_variable(t_env *curr)
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

static int	allocate_env_array(char ***new_envp, int env_count)
{
	*new_envp = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!*new_envp)
		return (0);
	return (1);
}

static char	**create_envp_array(t_msh *msh)
{
	int		env_count;
	char	**new_envp;
	t_env	*curr;
	int		i;
	
	env_count = count_env_nodes(msh->env);
	if (!allocate_env_array(&new_envp, env_count))
		return (NULL);
	
	i = 0;
	curr = msh->env;
	while (curr)
	{
		new_envp[i] = create_env_variable(curr);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
		curr = curr->next;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static int	update_envp_array(t_msh *msh)
{
	char	**new_envp;

	new_envp = create_envp_array(msh);
	if (!new_envp)
		return (0);

	if (msh->envp)
	{
		int i = 0;
		while (msh->envp[i])
		{
			if (msh->envp[i])
				free(msh->envp[i]);
			i++;
		}
		free(msh->envp);
	}

	msh->envp = new_envp;
	
	return (1);
}

void	ft_unset(t_msh *msh, t_cmd *cmd)
{
	int	i;
	int	changed;

	if (!cmd || !cmd->argv || !cmd->argv[1])
	{
		msh->state = 0;
		return;
	}
	
	changed = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (delete_env_var(msh, cmd->argv[i]))
			changed = 1;
		i++;
	}
	
	if (changed)
	{
		if (!update_envp_array(msh))
			msh->state = 1;
		else
			msh->state = 0;
	}
	else
		msh->state = 0;
}

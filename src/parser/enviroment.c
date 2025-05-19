/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:40 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/17 13:00:35 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_env_back(t_env **lst, t_env *new)
{
	t_env	*new_node;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	new_node = *lst;
	while (new_node->next != NULL)
		new_node = new_node->next;
	new_node->next = new;
}

static void	init_env_lst(t_env **lst, char **envp)
{
	t_env	*new;
	int		i;
	int		x;
	int		j;

	i = -1;
	while (envp[++i])
	{
		x = 0;
		new = malloc(sizeof(t_env));
		if (!new)
			return ;
		while (envp[i][x] != '=')
			x++;
		new->type = ft_substr(envp[i], 0, x);
		j = ++x;
		while (envp[i][j])
			j++;
		new->content = ft_substr(envp[i], x, (j - x));
		new->next = NULL;
		add_env_back(lst, new);
	}
}

t_env	*enviroment_lst(char **envp)
{
	t_env *env;

	if (!envp || !*envp)
		return (NULL);
	env = NULL;
	init_env_lst(&env, envp);
	return (env);
}

t_env	*create_env_lst(char **envp)
{
	t_env *env;

	if (!envp || !*envp)
		return (NULL);
	env = NULL;
	init_env_lst(&env, envp);
	return (env);
}
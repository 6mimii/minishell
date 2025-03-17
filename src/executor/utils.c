/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:47:53 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/03/17 17:25:18 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_cmd_ind(t_cmd *cmd)
{
	int		index;
	t_cmd	*current;

	index = 0;
	current = cmd;
	while (current)
	{
		current->index = index;
		index++;
		current = current->next;
	}
}

char	**get_path(void)
{
	char	*path_env;
	char	**paths;
	
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	return (paths);
}

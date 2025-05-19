/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_c.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:47:53 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/19 10:29:18 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	**get_path(t_msh *msh)
{
	char	*path_env;
	char	**paths;

	(void)msh;
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	return (paths);
}

void	wait_handler(t_msh *msh, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		msh->state = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		msh->state = 128 + WTERMSIG(status);
	else
		msh->state = 1;
}

int	check_dollar(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && str[i
				+ 1] != '$')
			return (1);
		i++;
	}
	return (0);
}

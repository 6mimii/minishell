/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_c.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:47:53 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/01 16:35:25 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	safe_fork(pid_t *pid)
{
	*pid = fork();
	if (*pid < 0)
		return (error_handl("Fork error"));
}

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

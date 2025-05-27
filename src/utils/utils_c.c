/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_c.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:47:53 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/27 16:22:09 by mdoudi-b         ###   ########.fr       */
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

	if (!msh || !msh->envp)
		return (NULL);
	if (!msh->env)
	{
		path_env = getenv("PATH");
		if (!path_env || path_env[0] == '\0')
			return (NULL);
		paths = ft_split(path_env, ':');
		return (paths);
	}
	path_env = get_env(msh, "PATH");
	if (!path_env || path_env[0] == '\0')
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
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

int	check_dollar_helper(const char *str, int i)
{
	return (str[i] == '$');
}

int	check_dollar(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (check_dollar_helper(str, i) && str[i + 1] && (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '{'))
			return (1);
		i++;
	}
	return (0);
}

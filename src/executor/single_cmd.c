/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:46:20 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/04/15 18:05:56 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_cmd(char **path, char *cmd, t_msh *msh)
{
	int		i;
	char	*cmd_joined;
	char	*aux;

	i = 0;
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	if (!path)
		error_and_exit(cmd, 127, msh);
	while (path[i])
	{
		aux = ft_strjoin(path[i], "/");
		cmd_joined = ft_strjoin(aux, cmd);
		if (access(cmd_joined, F_OK | X_OK) == 0)
		{
			free(aux);
			return (cmd_joined);
		}
		free(aux);
		free(cmd_joined);
		i++;
	}
	return (NULL);
}

void run_external_command(t_msh *msh, t_cmd *cmd, char **paths)
{
	char *full_path;
	
	if (!cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		free_and_exit("", msh, 0, false);
	}
	full_path = find_cmd(cmd->argv[0], paths, msh);
    if (!full_path)
		free_and_exit("Command not found", msh, 127, true);
	execve(full_path, cmd->argv, msh->envp);
	free(full_path);
	free_and_exit("execve failed", msh, EXIT_FAILURE, true);
}

void handle_single_command(t_msh *msh)
{
	pid_t	pid;
	
	if (msh->cmd->error)
		return;
	if (is_builtin(msh, msh->cmd))
		return;
	pid = fork();
	if (pid < 0)
		error_msh("Error creating pid", msh, 0);
	if (pid == 0)
	{
		if (msh->cmd->fd_in != STDIN_FILENO)
		{
			dup2(msh->cmd->fd_in, STDIN_FILENO);
			close(msh->cmd->fd_in);
		}
		if (msh->cmd->fd_out != STDOUT_FILENO)
		{
			dup2(msh->cmd->fd_out, STDOUT_FILENO);
			close(msh->cmd->fd_out);
		}
		run_external_command(msh, msh->cmd, msh->path);
	}
	wait_for_child(msh, pid);
}

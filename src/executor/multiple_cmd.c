/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:55:21 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 14:55:22 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	create_child(t_msh *msh, t_cmd *cmd, int *fd, int fd_in)
{
	if (cmd->error == 1)
		free_and_exit("", msh, EXIT_FAILURE, false);
	if (cmd->index == 0)
		close(fd[0]);
	if (!cmd->next)
		close(fd[1]);
	if (cmd->fd_in != 0)
		dup2(cmd->fd_in, 0);
	else if (cmd->fd_in == 0)
		dup2(fd_in, 0);
	if (cmd->fd_out != 1)
		dup2(cmd->fd_out, 1);
	else if (cmd->fd_out == 1)
		dup2(fd[1], 1);
	if (is_builtin(msh, cmd) == 0)
		free_and_exit("", msh, EXIT_SUCCESS, false);
	run_external_command(msh, cmd, msh->path);
}

void	multiple_cmds(t_msh *msh, int fd_in)
{
	pid_t	pid;
	int		fd[2];
	t_cmd	*tmp;

	tmp = msh->cmd;
	while (tmp)
	{
		if (pipe(fd))
			return (error_msh("Error creating pipe", msh, 1));
		pid = fork();
		if (pid == -1)
			return (error_msh("Error creating child process", msh, 1));
		if (pid == 0)
			create_child(msh, tmp, fd, fd_in);
		else
		{
			wait_handler(msh, pid);
			close(fd[1]);
		}
		tmp = tmp->next;
		close(fd_in);
		fd_in = dup(fd[0]);
		close(fd[0]);
	}
}

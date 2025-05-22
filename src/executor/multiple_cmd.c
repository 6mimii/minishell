/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:46:36 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/22 20:53:24 by mimi-notebo      ###   ########.fr       */
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

static int count_commands(t_cmd *cmd)
{
    int num_commands = 0;
    t_cmd *tmp = cmd;

    while (tmp)
    {
        num_commands++;
        tmp = tmp->next;
    }
    return num_commands;
}

static void handle_parent_process(t_msh *msh, pid_t pid, int *fd, int *fd_in)
{
    wait_handler(msh, pid);
    close(fd[1]);
    close(*fd_in);
    *fd_in = dup(fd[0]);
    close(fd[0]);
}

void multiple_cmds(t_msh *msh, int fd_in)
{
    pid_t pid;
    int fd[2];
    t_cmd *tmp;
    int num_commands = count_commands(msh->cmd);

    int i = 0;
    tmp = msh->cmd;
    while (i < num_commands)
    {
        if (pipe(fd))
            return (error_msh("Error creating pipe", msh, 1));
        pid = fork();
        if (pid == -1)
            return (error_msh("Error creating child process", msh, 1));
        if (pid == 0)
            create_child(msh, tmp, fd, fd_in);
        else
            handle_parent_process(msh, pid, fd, &fd_in);
        tmp = tmp->next;
        i++;
    }
}

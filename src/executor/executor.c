/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:45:06 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/04/02 12:30:21 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_builtin(t_msh *msh, t_cmd *cmd) // TODO, manca tutta la parte seria
{
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(msh, cmd), 0);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(msh, cmd, cmd->fd_out), 0);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (ft_exit(msh, cmd), 0);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(msh, cmd), 0);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd(msh, cmd), 0);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_unset(msh, cmd), 0);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(msh, cmd), 0);
	return (1);
}

void	executor(t_msh *msh)
{
	int	fd[2];
	int	fd_in;
	
	g_signal = 1;
	fd[0] = dup(0);
	fd[1] = dup(1);
	set_cmd_ind(msh->cmd); // FATTO set command index
	fd_in = msh->cmd->fd_in;
	msh->path = get_path(); // FATTO	get_path
	init_signals(msh); // TODO initialisaition of the signals
	if (msh->cmd_len == 1)
		one_cmd_handl(msh); // TODO
	else
		multiple_cmds(msh, fd_in); // TODO
	g_signal = 0;
	dup2(fd[0], 0);
	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
	init_signals(msh);
}

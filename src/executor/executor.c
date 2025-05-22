/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:45:06 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/22 02:02:10 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(t_msh *msh, t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
		
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(msh, cmd), 0);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(msh, cmd, cmd->fd_out), 0);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
	{
		ft_putendl_fd("exit", 2);
		if (!cmd->argv[1]) {
			exit(0);
		}
		int i = 0;
		int exit_val = 0;
		while (cmd->argv[1][i] && (cmd->argv[1][i] == ' ' || cmd->argv[1][i] == '\t'))
			i++;
		if (cmd->argv[1][i] == '-') {
			i++;
		}
		else if (cmd->argv[1][i] == '+') {
			i++;
			}
		int start_i = i;
		while (cmd->argv[1][i] >= '0' && cmd->argv[1][i] <= '9')
			i++;
		if (cmd->argv[1][i] == '\0' && i > start_i)
		{
			if (cmd->argv[2])
			{
				ft_putendl_fd("minishell: exit: too many arguments", 2);
				msh->state = 1;
				return (0);
				}
			exit_val = ft_atoi(cmd->argv[1]);
			exit_val = exit_val % 256;
			if (exit_val < 0)
				exit_val += 256;
			exit(exit_val);
		}
		else {
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd->argv[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit(2);
		}
		return (0);
	}
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
	{
		ft_export(msh, cmd);
		msh->state = 0;
		return (0);
	}
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd(msh, cmd), 0);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
	{
		ft_unset(msh, cmd);
		msh->state = 0;
		return (0);
	}
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(msh, cmd, cmd->argv[1]), 0);
	return (1);
}

void	executor(t_msh *msh)
{
	int	saved_stdin;
	int	saved_stdout;
	int	fd_in;

	if (!msh || !msh->cmd || !msh->cmd->argv || !msh->cmd->argv[0])
		return;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	set_cmd_ind(msh->cmd);
	fd_in = msh->cmd->fd_in;
	msh->path = get_path(msh);
	setup_signals(msh);
	g_signal = 1;
	if (msh->cmd_len == 1)
		handle_single_command(msh);
	else
		multiple_cmds(msh, fd_in);
	g_signal = 0;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	setup_signals(msh);
	if (msh->path)
	{
		free_matrix(msh->path);
		msh->path = NULL;
	}
}

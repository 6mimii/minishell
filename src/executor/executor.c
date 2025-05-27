/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:45:06 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/27 16:28:01 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int handle_builtin_command(t_msh *msh, t_cmd *cmd, char *cmd_name)
{
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (ft_cd(msh, cmd), 0);
	else if (ft_strcmp(cmd_name, "echo") == 0)
		return (ft_echo(msh, cmd, cmd->fd_out), 0);
	else if (ft_strcmp(cmd_name, "exit") == 0)
		return (ft_exit(msh, cmd), 0);
	else if (ft_strcmp(cmd_name, "export") == 0)
		return (ft_export(msh, cmd), msh->state = 0, 0);
	else if (ft_strcmp(cmd_name, "pwd") == 0)
		return (ft_pwd(msh, cmd), 0);
	else if (ft_strcmp(cmd_name, "unset") == 0)
		return (ft_unset(msh, cmd), msh->state = 0, 0);
	else if (ft_strcmp(cmd_name, "env") == 0)
		return (ft_env(msh, cmd, cmd->argv[1]), 0);
	return (1);
}

int	is_builtin(t_msh *msh, t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	return (handle_builtin_command(msh, cmd, cmd->argv[0]));
}

static void	cleanup_executor(t_msh *msh, int saved_stdin, int saved_stdout)
{
	g_signal = 0;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	setup_signals();
	if (msh->path)
	{
		free_matrix(msh->path);
		msh->path = NULL;
	}
}

void	executor(t_msh *msh)
{
	int	saved_stdin;
	int	saved_stdout;
	int	fd_in;

	if (!msh || !msh->cmd || !msh->cmd->argv || !msh->cmd->argv[0])
		return ;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	set_cmd_ind(msh->cmd);
	fd_in = msh->cmd->fd_in;
	msh->path = get_path(msh);
	setup_signals();
	g_signal = 1;
	if (msh->cmd_len == 1)
		handle_single_command(msh);
	else
		multiple_cmds(msh, fd_in);
	cleanup_executor(msh, saved_stdin, saved_stdout);
}

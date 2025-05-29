/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:25:21 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:28:04 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(t_msh *msh, t_cmd *cmd)
{
	if (!cmd->argv[1])
	{
		free_and_exit_ex(msh);
		exit(0);
	}
	if (cmd->argv[2] && all_nbr(cmd->argv[1]) == true)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		msh->state = 1;
		return ;
	}
	exit_extra_options(msh, cmd);
}

void	process_export_arg(t_msh *msh, char *arg)
{
	if (ft_strchr(arg, '='))
		add_vars(msh, arg);
	else if (!get_env_type(msh, arg))
		add_env(msh, arg, NULL);
}

void	ft_export(t_msh *msh, t_cmd *cmd)
{
	int	i;

	if (!cmd->argv[1])
		return (print_export(msh->env, cmd->fd_out));
	i = 1;
	while (cmd->argv[i])
	{
		if (!parse_export(msh, cmd->argv[i]))
			msh->state = 1;
		else
			process_export_arg(msh, cmd->argv[i]);
		i++;
	}
}

void	setup_child_redirections(t_msh *msh)
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
}

void	handle_single_command(t_msh *msh)
{
	pid_t	pid;

	if (!msh || !msh->cmd || msh->cmd->error)
		return ;
	if (!msh->cmd->argv || !msh->cmd->argv[0])
		return ;
	if (is_builtin(msh, msh->cmd) == 0)
	{
		msh->state = 0;
		return ;
	}
	pid = fork();
	if (pid < 0)
		error_msh("Error creating pid", msh, 0);
	if (pid == 0)
	{
		setup_child_redirections(msh);
		run_external_command(msh, msh->cmd, msh->path);
	}
	wait_handler(msh, pid);
}

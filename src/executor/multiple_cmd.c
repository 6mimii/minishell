/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:46:36 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/21 17:45:23 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	create_child(t_msh *msh, t_cmd *cmd, int *fd, int fd_in)
{
	if (cmd->error == 1)
		free_and_exit("", msh, EXIT_FAILURE, false);
		
	// Comprobamos si el comando es "exit" antes de configurar los descriptores
	if (cmd->argv && cmd->argv[0] && ft_strcmp(cmd->argv[0], "exit") == 0)
	{
		// Salimos directamente para que funcione exit en tuberías
		ft_putendl_fd("exit", 2);
		if (!cmd->argv[1])
			exit(0);
			
		// Verificamos si es un número
		int i = 0;
		int is_num = 1;
		int has_digits = 0;
		
		// Saltamos espacios y manejo de signo
		while (cmd->argv[1][i] && (cmd->argv[1][i] == ' ' || cmd->argv[1][i] == '\t'))
			i++;
			
		if (cmd->argv[1][i] == '+' || cmd->argv[1][i] == '-')
			i++;
			
		while (cmd->argv[1][i])
		{
			if (cmd->argv[1][i] >= '0' && cmd->argv[1][i] <= '9')
				has_digits = 1;
			else
				is_num = 0;
			i++;
		}
			
		if (is_num && has_digits)
		{
			if (cmd->argv[2]) {
				ft_putendl_fd("minishell: exit: too many arguments", 2);
				exit(1);
			}
			exit(ft_atoi(cmd->argv[1]) % 256);
		}
		else {
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd->argv[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit(2);
		}
	}
	
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

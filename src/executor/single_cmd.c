/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   */
/*   Created: 2025/03/07 12:46:20 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/18 19:13:32 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_cmd(char **path, char *cmd, t_msh *msh)
{
	int		i;
	char	*cmd_joined;
	char	*aux;

	i = 0;
	(void)msh;
	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/') || 
		(cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	if (!path)
		return (NULL);
	while (path && path[i])
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

void	run_external_command(t_msh *msh, t_cmd *cmd, char **paths)
{
	char	*full_path;

	if (!cmd || !cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		ft_putstr_fd("Minishell: command missing\n", 2);
		free_and_exit("", msh, 0, false);
	}
	
	full_path = find_cmd(paths, cmd->argv[0], msh);
	if (!full_path)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		free_and_exit("", msh, 127, false);
	}
	
	if (!msh->envp) {
		// Fallback to system environment if our environment is NULL
		extern char **environ;
		execve(full_path, cmd->argv, environ);
	} else {
		execve(full_path, cmd->argv, msh->envp);
	}
	
	free(full_path);
	free_and_exit("execve failed", msh, EXIT_FAILURE, true);
}

void	handle_single_command(t_msh *msh)
{
	pid_t	pid;

	// Debug
	write(2, "In handle_single_command\n", 25);
	
	if (!msh || !msh->cmd)
	{
		write(2, "Error: msh or cmd is NULL\n", 26);
		return;
	}
	
	if (msh->cmd->error)
	{
		write(2, "Cmd has error flag set\n", 23);
		return;
	}
	
	if (!msh->cmd->argv || !msh->cmd->argv[0])
	{
		write(2, "Cmd argv is NULL or empty\n", 26);
		return;
	}
	
	write(2, "Command argv[0]: ", 17);
	write(2, msh->cmd->argv[0], ft_strlen(msh->cmd->argv[0]));
	write(2, "\n", 1);
	
	// Debugging: Log the command and arguments being processed
	write(2, "Debug: Command being processed: ", 30);
	write(2, msh->cmd->argv[0], ft_strlen(msh->cmd->argv[0]));
	write(2, "\n", 1);
	for (int i = 1; msh->cmd->argv[i]; i++) {
		write(2, "Arg: ", 5);
		write(2, msh->cmd->argv[i], ft_strlen(msh->cmd->argv[i]));
		write(2, "\n", 1);
	}

	// Debugging: Log the command and arguments being passed to builtins
	write(2, "Debug: Command: ", 15);
	write(2, msh->cmd->argv[0], ft_strlen(msh->cmd->argv[0]));
	write(2, "\n", 1);
	for (int i = 1; msh->cmd->argv[i]; i++) {
		write(2, "Arg: ", 5);
		write(2, msh->cmd->argv[i], ft_strlen(msh->cmd->argv[i]));
		write(2, "\n", 1);
	}

	if (is_builtin(msh, msh->cmd) == 0) {
		write(2, "Builtin executed successfully\n", 30);
		msh->state = 0; // Indicar éxito en la ejecución del builtin
		return;
	}
	
	write(2, "Forking for external command\n", 29);
	pid = fork();
	if (pid < 0)
	{
		write(2, "Fork failed\n", 12);
		error_msh("Error creating pid", msh, 0);
	}
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
	wait_handler(msh, pid);
}

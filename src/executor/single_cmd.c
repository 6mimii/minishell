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

	if (!cmd || !cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0' || is_only_spaces(cmd->argv[0]))
	{
		if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0' || is_only_spaces(cmd->argv[0]))
			free_and_exit("", msh, 0, false);
		else
		{
			ft_putstr_fd("Minishell: command missing\n", 2);
			free_and_exit("", msh, 0, false);
		}
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
		extern char **environ;
		if (execve(full_path, cmd->argv, environ) == -1) {
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			free(full_path);
			free_and_exit("", msh, 126, false);
		}
	} else {
		if (execve(full_path, cmd->argv, msh->envp) == -1) {
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			free(full_path);
			free_and_exit("", msh, 126, false);
		}
	}
	
	free(full_path);
	free_and_exit("execve failed", msh, EXIT_FAILURE, true);
}

void handle_exit(t_msh *msh, t_cmd *cmd) {
    int exit_code = 0;

    if (cmd->argv[1]) {
        int i = 0;
        while (cmd->argv[1][i]) {
            if (!ft_isdigit(cmd->argv[1][i])) {
                ft_putstr_fd("Minishell: exit: numeric argument required\n", 2);
                free_and_exit("", msh, 255, false);
            }
            i++;
        }
        if (cmd->argv[2]) {
            ft_putstr_fd("Minishell: exit: too many arguments\n", 2);
            msh->state = 1;
            return;
        }
        exit_code = ft_atoi(cmd->argv[1]);
    }
    free_and_exit("", msh, exit_code, false);
}

void	handle_single_command(t_msh *msh)
{
	pid_t	pid;

    if (!msh || !msh->cmd)
        return;

    if (msh->cmd->error)
        return;

    if (!msh->cmd->argv || !msh->cmd->argv[0] || is_only_spaces(msh->cmd->argv[0]))
        return;

    if (ft_strcmp(msh->cmd->argv[0], "exit") == 0) {
        handle_exit(msh, msh->cmd);
        return;
    }

    if (is_builtin(msh, msh->cmd) == 0) {
        msh->state = 0;
        return;
    }

    pid = fork();
    if (pid < 0)
        error_msh("Error creating pid", msh, 0);
    if (pid == 0) {
        if (msh->cmd->fd_in != STDIN_FILENO) {
            dup2(msh->cmd->fd_in, STDIN_FILENO);
            close(msh->cmd->fd_in);
        }
        if (msh->cmd->fd_out != STDOUT_FILENO) {
            dup2(msh->cmd->fd_out, STDOUT_FILENO);
            close(msh->cmd->fd_out);
        }
        run_external_command(msh, msh->cmd, msh->path);
    }
    wait_handler(msh, pid);
}

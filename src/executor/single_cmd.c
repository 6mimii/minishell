#include "../minishell.h"

static int	is_absolute_or_relative_path(char *cmd)
{
	return (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/') || (cmd[0] == '.'
			&& cmd[1] == '.' && cmd[2] == '/'));
}

static char	*search_in_path(char **path, char *cmd)
{
	int		i;
	char	*cmd_joined;
	char	*aux;

	i = 0;
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

char	*find_cmd(char **path, char *cmd, t_msh *msh)
{
	(void)msh;
	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (is_absolute_or_relative_path(cmd))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	if (!path)
		return (NULL);
	return (search_in_path(path, cmd));
}

static void	execute_command(t_msh *msh, t_cmd *cmd, char *full_path)
{
	extern char	**environ;

	if (!msh->envp)
		execve(full_path, cmd->argv, environ);
	else
		execve(full_path, cmd->argv, msh->envp);
	free(full_path);
	free_and_exit("execve failed", msh, EXIT_FAILURE, true);
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
	execute_command(msh, cmd, full_path);
}

static void	setup_child_redirections(t_msh *msh)
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

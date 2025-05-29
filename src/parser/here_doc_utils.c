/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:17:16 by mimi-notebo       #+#    #+#             */
/*   Updated: 2025/05/29 16:15:28 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_limit_reached(char *line, char *limit, int len);

static void	new_handler(int sig)
{
	if (sig == SIGINT)
		ctrl_c_hd(sig);
}

void	handle_ctrl_c(t_msh *msh)
{
	signal(SIGINT, new_handler);
	(void)msh;
}

void	wait_hd(t_token *tok, t_cmd *cmd, t_msh *msh, int fd)
{
	int	stat;

	(void)msh;
	waitpid(0, &stat, 0);
	if (WEXITSTATUS(stat) == 130)
	{
		cmd->error = 1;
		msh->state = 130;
	}
	else if (WEXITSTATUS(stat) == 1)
	{
		ft_putstr_fd(CTRLD_HD, 1);
		ft_putstr_fd(tok->content, 1);
		ft_putendl_fd("')", 1);
	}
	close(fd);
	cmd->fd_in = open(".here_doc.tmp", O_RDONLY);
}

void	here_doc(char *limit, t_cmd *new, t_msh *msh, int fd)
{
	char	*line;
	int		len;

	(void)new;
	len = ft_strlen(limit);
	line = readline("> ");
	while (line)
	{
		if (is_limit_reached(line, limit, len))
		{
			free(line);
			break ;
		}
		line = process_heredoc_line(line, msh, fd);
		line = readline("> ");
	}
	if (!line)
		free_and_exit_hd(msh, new, 1);
	free_and_exit_hd(msh, new, 0);
}

static int	is_limit_reached(char *line, char *limit, int len)
{
	return (ft_strncmp(line, limit, len) == 0 && !line[len]);
}

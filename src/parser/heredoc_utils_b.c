/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:29:44 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:41:01 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_heredoc_line(char *line, t_msh *msh, int fd)
{
	line = expand_heredoc(line, msh);
	if (line)
	{
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (line);
}

static void	setup_heredoc_fd(t_cmd *new, t_msh *msh, int *fd)
{
	(void)msh;
	if (new->fd_in > 2)
		close(new->fd_in);
	*fd = open(".here_doc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		error_files(".here_doc.tmp", strerror(errno));
		new->error = 1;
	}
}

void	set_heredoc(t_token **tok, t_cmd *new, t_msh *msh)
{
	pid_t	pid;
	int		fd;

	if (!*tok || !(*tok)->next)
		return (error_msh(UNEXPECTED_EOF, msh, 2), new->error = 1, (void)0);
	*tok = (*tok)->next;
	if (new->error == 0)
	{
		setup_heredoc_fd(new, msh, &fd);
		if (new->error == 0)
		{
			pid = fork();
			if (pid == 0)
			{
				handle_ctrl_c(msh);
				here_doc((*tok)->content, new, msh, fd);
			}
			else if (pid > 0)
				wait_hd(*tok, new, msh, fd);
		}
	}
	if (*tok && (*tok)->next)
		*tok = (*tok)->next;
}

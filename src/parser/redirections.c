/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:40:36 by mimi-notebo       #+#    #+#             */
/*   Updated: 2025/05/27 16:43:16 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_redirection_error(t_token **tok, t_cmd *new, t_msh *msh)
{
	if (!*tok || !(*tok)->next)
		return (error_msh(UNEXPECTED_EOF, msh, 2), new->error = 1, 0);
	*tok = (*tok)->next;
	return (1);
}

static void	finalize_redirection(t_token **tok)
{
	if (*tok && (*tok)->next)
		*tok = (*tok)->next;
}

void	set_outfile(t_token **tok, t_cmd *new, t_msh *msh)
{
	if (!handle_redirection_error(tok, new, msh))
		return ;
	if (new->error == 0)
	{
		if (new->fd_out > 2)
			close(new->fd_out);
		new->fd_out = open((*tok)->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (new->fd_out == -1)
			error_files((*tok)->content, strerror(errno));
		new->error = 1;
		msh->state = 1;
	}
	finalize_redirection(tok);
}

void	set_infile(t_token **tok, t_cmd *new, t_msh *msh)
{
	if (!handle_redirection_error(tok, new, msh))
		return ;
	if (new->error == 0)
	{
		if (new->fd_in > 2)
			close(new->fd_in);
		new->fd_in = open((*tok)->content, O_RDONLY);
		if (new->fd_in == -1)
			error_files((*tok)->content, strerror(errno));
		new->error = 1;
		msh->state = 1;
	}
	finalize_redirection(tok);
}

void	set_append(t_token **tok, t_cmd *new, t_msh *msh)
{
	if (!handle_redirection_error(tok, new, msh))
		return ;
	if (new->error == 0)
	{
		if (new->fd_out > 2)
			close(new->fd_out);
		new->fd_out = open((*tok)->content, O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (new->fd_out == -1)
			error_files((*tok)->content, strerror(errno));
		new->error = 1;
		msh->state = 1;
	}
	finalize_redirection(tok);
}

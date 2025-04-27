/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:40:36 by mimi-notebo       #+#    #+#             */
/*   Updated: 2025/04/27 16:27:53 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_outfile(t_token **tok, t_cmd *new, t_msh *msh)
{
	*tok = (*tok)->next;
	if (new->error == 0)
	{
		if (new->fd_out > 2)
			close(new->fd_out);
		new->fd_out = open((*tok)->content,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (new->fd_out == -1)
		{
			error_files((*tok)->content, strerror(errno));
			new->error = 1;
			msh->state = 1;
		}
	}
	*tok = (*tok)->next;
}

void	set_infile(t_token **tok, t_cmd *new, t_msh *msh)
{
	*tok = (*tok)->next;
	if (new->error == 0)
	{
		if (new->fd_in > 2)
			close(new->fd_in);
		new->fd_in = open((*tok)->content, O_RDONLY);
		if (new->fd_in == -1)
		{
			error_files((*tok)->content, strerror(errno));
			new->error = 1;
			msh->state = 1;
		}
	}
	*tok = (*tok)->next;
}

void	set_append(t_token **tok, t_cmd *new, t_msh *msh)
{
	*tok = (*tok)->next;
	if (new->error == 0)
	{
		if (new->fd_out > 2)
			close(new->fd_out);
		new->fd_out = open((*tok)->content,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (new->fd_out == -1)
		{
			error_files((*tok)->content, strerror(errno));
			new->error = 1;
			msh->state = 1;
		}
	}
	*tok = (*tok)->next;
}
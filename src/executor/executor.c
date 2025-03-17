/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:45:06 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/03/17 17:25:46 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executor(t_msh *msh)
{
	int	fd[2];
	int	fd_in;
	
	g_signal = 1;
	fd[0] = dup(0);
	fd[1] = dup(1);
	set_cmd_ind(msh->cmd); // FATTO set command index
	fd_in = msh->cmd->fd_in;
	msh->path = get_path(); // FATTO	get_path
	init_signals(msh); // TODO initialisaition of the signals
	if (msh->cmd_len == 1)
		one_cmd_handl(msh); // TODO
	else
		multiple_cmds(msh, fd_in); // TODO
	g_signal = 0;
	dup2(fd[0], 0);
	dup2(fd[1], 1);
	init_signals(msh);	
}

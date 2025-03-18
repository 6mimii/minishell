/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:46:20 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/03/18 17:36:25 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	one_cmd_handl(t_msh *msh)
{
	pid_t	pid;

	if (msh->cmd->error == 1)
		return ;
	if (is_builtin(msh, msh->cmd) == 0) //TODO is_builtin
		return ;
	safe_fork(&pid);
	if (pid == 0) //TODO processo figlio quindi con exe e tutto l'ambradan
	{}
	easy_wait(); //TODO wait del pocesso padre
}

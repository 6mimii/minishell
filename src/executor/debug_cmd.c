/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:00:00 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/21 17:45:23 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_debug_cmd(t_msh *msh, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd->argv[0])
		return ;
	ft_putendl_fd("\n--- DEBUG PRINT COMMAND ARGS ---", STDOUT_FILENO);
	ft_putstr_fd("Command: ", STDOUT_FILENO);
	ft_putendl_fd(cmd->argv[0], STDOUT_FILENO);
	ft_putendl_fd("Arguments:", STDOUT_FILENO);
	i = 1;
	while (cmd->argv[i])
	{
		ft_putstr_fd("[", STDOUT_FILENO);
		ft_putstr_fd(ft_itoa(i), STDOUT_FILENO);
		ft_putstr_fd("]: '", STDOUT_FILENO);
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		ft_putendl_fd("'", STDOUT_FILENO);
		i++;
	}
	ft_putendl_fd("--- END DEBUG PRINT ---\n", STDOUT_FILENO);
	msh->state = 0;
}

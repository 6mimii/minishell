/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:49:58 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/01 16:24:03 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(t_msh *msh, t_cmd *cmd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (error_msh("pwd: Cannot find the pwd", msh, 0));
	ft_putendl_fd(pwd, cmd->fd_out);
	free(pwd);
	msh->state = 0;
}

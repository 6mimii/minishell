/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:49:58 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/04/15 18:21:08 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* TODO sistemare per bene e check */

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:47:36 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/01 17:27:31 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_minishell(char *msg, t_msh *msh, int state)
{
	ft_putstr_fd("Minishell : ", 2);
	ft_putendl_fd(msg, 2);
	msh->state = state;
}

void	error_files(char *name, char *msg)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}

void	free_exit(char *msg, t_msh *msh, int state, bool print)
{
	if (print == true)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putendl_fd(msg, 2);
	}
	free_env(msh->env);
	free_msh(msh);
	exit(state);
}
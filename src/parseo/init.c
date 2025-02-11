/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsaffiri <fsaffiri@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:47 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/02/11 15:30:48 by fsaffiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_msh(char **envp, t_msh *msh)
{
	msh->env = enviroment_lst(envp);
	msh->envp = envp;
}

void get_input(t_msh *msh)
{
	msh->input = readline("Mimishell% ");
	if (!msh->input)
		crtl_d();
	else
		add_history(msh->input);
		msh->tokens = set_tokens;
}

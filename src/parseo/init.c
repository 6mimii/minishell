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

#include "../minishell.h"

static int clean_tokens (t_msh *msh)
{
	if (!msh->tokens || msh->parse_error)
		return (0);
	expand_flag(msh->tokens);
	expand_tokens(&msh->tokens, msh);
	
}

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
	while (msh->input)
	{
		if (msh->input[0] == '\0')
			free(msh->input);
		else
		{
			add_history(msh->input);
			msh->tokens = set_tokens(msh->input, msh);
			if (clean_tokens(msh))
			{

			}
		}
		msh->input = readline("Mimishell% ");
		if(!msh->input)
			ctrl_d();
	}
	free_msh(msh);
}

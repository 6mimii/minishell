/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:47 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/25 22:57:54 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	clean_tokens(t_msh *msh)
{
	if (!msh->tokens || msh->parse_error)
		return (0);
	
	expand_flag(msh->tokens);
	expand_tokens(&msh->tokens, msh);
	
	return (1);
}

static void reset_msh_for_next_command(t_msh *msh)
{
    char **unset_vars = msh->unset_vars;
    t_env *env = msh->env;
    char **envp = msh->envp;
    int state = msh->state;
    
    if (msh->tokens)
        free_tokens(&msh->tokens);
    if (msh->cmd)
        free_commands(&msh->cmd);
    if (msh->input)
    {
        free(msh->input);
        msh->input = NULL;
    }
    if (msh->path)
    {
        free_matrix(msh->path);
        msh->path = NULL;
    }
    
    msh->env = env;
    msh->envp = envp;
    msh->unset_vars = unset_vars;
    msh->cmd_len = 0;
    msh->parse_error = 0;
    msh->state = state;
    msh->cmd = NULL;
    msh->tokens = NULL;
    msh->path = NULL;
}

void	init_msh(char **envp, t_msh *msh)
{
	msh->env = create_env_lst(envp);
	msh->envp = envp;
	msh->cmd_len = 0;
	msh->parse_error = 0;
	msh->state = 0;
	msh->cmd = NULL;
	msh->tokens = NULL;
	msh->path = NULL;
	msh->unset_vars = NULL;
}

void	get_input(t_msh *msh)
{
	msh->input = readline("Mimishell% ");
	if (!msh->input)
		ctrl_d();
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
				get_command(msh);
				if (msh->cmd)
					executor(msh);
				reset_msh_for_next_command(msh);
			}
		}
		msh->input = readline("Mimishell% ");
		if (!msh->input)
			ctrl_d();
	}
	free_msh(msh);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:56:30 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:41:51 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	clean_tokens(t_msh *msh)
{
	if (!msh->tokens || msh->parse_error)
		return (0);
	expand_flag(msh->tokens);
	expand_tokens(&msh->tokens, msh);
	return (1);
}

static void	free_msh_temporaries(t_msh *msh)
{
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
}

void	reset_msh_for_next_command(t_msh *msh)
{
	char	**unset_vars;
	t_env	*env;
	char	**envp;
	char	**original_envp;
	int		state;

	unset_vars = msh->unset_vars;
	env = msh->env;
	envp = msh->envp;
	original_envp = msh->original_envp;
	state = msh->state;
	free_msh_temporaries(msh);
	msh->env = env;
	msh->envp = envp;
	msh->original_envp = original_envp;
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
	msh->original_envp = envp;
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
	g_signal = 0;
	setup_signals();
	msh->input = readline("Mimishell% ");
	if (!msh->input)
		ctrl_d(msh);
	while (msh->input)
	{
		process_command_input(msh);
		g_signal = 0;
		setup_signals();
		msh->input = readline("Mimishell% ");
		if (!msh->input)
			ctrl_d(msh);
	}
	free_msh(msh);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:47 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/27 17:35:24 by mdoudi-b         ###   ########.fr       */
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

static void	reset_msh_for_next_command(t_msh *msh)
{
	char	**unset_vars;
	t_env	*env;
	char	**envp;
	int		state;

	unset_vars = msh->unset_vars;
	env = msh->env;
	envp = msh->envp;
	state = msh->state;
	free_msh_temporaries(msh);
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

static void	process_command_input(t_msh *msh)
{
	if (msh->input[0] == '\0')
		free(msh->input);
	else
	{
		add_history(msh->input);
		msh->tokens = set_tokens(msh->input, msh);
		if (msh->parse_error)
			reset_msh_for_next_command(msh);
		else if (clean_tokens(msh))
		{
			get_command(msh);
			if (msh->cmd)
				executor(msh);
			reset_msh_for_next_command(msh);
		}
	}
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

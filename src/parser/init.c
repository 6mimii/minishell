/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:47 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/22 02:02:10 by mimi-notebo      ###   ########.fr       */
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

static int	is_only_whitespace(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

void	init_msh(char **envp, t_msh *msh)
{
	msh->env = create_env_lst(envp);
	msh->envp = envp;
	msh->cmd_len = 0;
	msh->parse_error = 0;
	msh->state = 0;
	msh->exit_requested = 0;
	msh->exit_code = 0;
	msh->cmd = NULL;
	msh->tokens = NULL;
	msh->path = NULL;
}

void	get_input(t_msh *msh)
{
	g_sigint_received = 0;
	g_signal = 0;
	setup_signals(msh);
	msh->input = readline("Mimishell% ");
	if (!msh->input)
		ctrl_d();
	while (msh->input)
	{
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			free(msh->input);
			msh->input = NULL;
		}
		else if (msh->input && (msh->input[0] == '\0' || is_only_whitespace(msh->input)))
		{
			free(msh->input);
			msh->input = NULL;
		}
		else if (msh->input)
		{
			add_history(msh->input);
			msh->tokens = set_tokens(msh->input, msh);
			if (clean_tokens(msh))
			{
				get_command(msh);
				if (msh->cmd)
				{
					executor(msh);
					if (msh->exit_requested)
					{
						free_msh(msh);
						exit(msh->exit_code);
						}
					}
				}
			free(msh->input);
			msh->input = NULL;
		}
		if (msh->cmd)
		{
			free_commands(&msh->cmd);
			msh->cmd_len = 0;
		}
		if (msh->tokens)
		{
			free_tokens(&msh->tokens);
		}
		g_sigint_received = 0;
		g_signal = 0;
		setup_signals(msh);
		msh->input = readline("Mimishell% ");
		if (!msh->input)
			ctrl_d();
	}
	free_msh(msh);
}

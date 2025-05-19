/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:47 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/19 10:41:07 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	clean_tokens(t_msh *msh)
{
	// Debug
	write(2, "Cleaning tokens...\n", 19);
	
	if (!msh->tokens || msh->parse_error)
	{
		write(2, "No tokens to clean or parse error\n", 34);
		return (0);
	}
	
	// Count tokens before expansion
	{
		int count = 0;
		t_token *tmp = msh->tokens;
		char num[12];
		
		while (tmp) {
			count++;
			tmp = tmp->next;
		}
		write(2, "Pre-clean token count: ", 23);
		sprintf(num, "%d", count);
		write(2, num, ft_strlen(num));
		write(2, "\n", 1);
	}
	
	write(2, "Expanding flags...\n", 19);
	expand_flag(msh->tokens);
	
	write(2, "Expanding tokens...\n", 20);
	expand_tokens(&msh->tokens, msh);
	
	// Count tokens after expansion
	{
		int count = 0;
		t_token *tmp = msh->tokens;
		char num[12];
		
		while (tmp) {
			count++;
			tmp = tmp->next;
		}
		write(2, "Post-clean token count: ", 24);
		sprintf(num, "%d", count);
		write(2, num, ft_strlen(num));
		write(2, "\n", 1);
	}
	
	return (1);
}

void	init_msh(char **envp, t_msh *msh)
{
	// Check if envp is valid
	if (!envp) {
		write(2, "Warning: envp is NULL\n", 22);
		extern char **environ;
		envp = environ;
	}
	
	msh->env = create_env_lst(envp);
	if (!msh->env) {
		write(2, "Warning: Failed to create environment list\n", 42);
	}
	
	msh->envp = envp;
	msh->cmd_len = 0;
	msh->parse_error = 0;
	msh->state = 0;
	msh->cmd = NULL;
	msh->tokens = NULL;
	msh->path = NULL;
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
			}
		}
		msh->input = readline("Mimishell% ");
		if (!msh->input)
			ctrl_d();
	}
	free_msh(msh);
}

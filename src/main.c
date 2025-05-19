/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:30:30 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/18 18:37:01 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_msh	msh;

	(void)ac;
	(void)av;
	
	// Debug message
	write(2, "Starting minishell...\n", 22);
	
	init_msh(envp, &msh);
	
	// Debug - check if envp is valid
	if (envp && envp[0])
	{
		write(2, "Environment OK\n", 15);
	}
	else
	{
		write(2, "Environment is empty or NULL\n", 29);
	}
	
	get_input(&msh);
}

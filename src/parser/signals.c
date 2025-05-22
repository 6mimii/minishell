/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:22:29 by mimi-notebo       #+#    #+#             */
/*   Updated: 2025/05/22 02:02:10 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal = 0;

volatile sig_atomic_t	g_sigint_received = 0;

void	ctrl_c(void)
{
	write(STDOUT_FILENO, "\n", 1);
	g_sigint_received = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (g_signal == 0)
		{
			ctrl_c();
		}
		else if (g_signal == 1)
		{
			write(STDOUT_FILENO, "\n", 1);
		}
	}
	else if (sig == SIGQUIT && g_signal == 1)
	{
		ft_putendl_fd("Quit (core dumped)", 2);
	}
}

void	ctrl_d(void)
{
	ft_putstr_fd("exit\n", 1);
	exit(0);
}

void	setup_signals(t_msh *msh)
{
	struct sigaction	act;

	act.sa_handler = &sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGINT, &act, NULL) == -1)
		error_msh("Error: sigaction", msh, 1);
	act.sa_handler = (g_signal == 0) ? SIG_IGN : &sig_handler;
	if (sigaction(SIGQUIT, &act, NULL) == -1)
		error_msh("Error: sigaction", msh, 1);
	act.sa_handler = SIG_IGN;
	if (sigaction(SIGTSTP, &act, NULL) == -1)
		error_msh("Error: sigaction", msh, 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:22:29 by mimi-notebo       #+#    #+#             */
/*   Updated: 2025/05/27 15:55:45 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <termios.h>

int	g_signal = 0;

void	ctrl_c(void)
{
	if (g_signal == 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_signal == 1)
		write(1, "\n", 1);
}

static void	sig_handler_interactive(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ctrl_d(t_msh *msh)
{
	ft_putstr_fd("exit\n", 1);
	free_msh(msh);
	exit(0);
}

static void	sig_handler_exec(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		write(1, "\n", 1);
	}
	else if (sig == SIGQUIT)
	{
		g_signal = 131;
		ft_putendl_fd("Quit (core dumped)", 2);
	}
}

void	setup_signals(void)
{
	if (g_signal == 0 || g_signal == 130)
	{
		signal(SIGINT, sig_handler_interactive);
		signal(SIGQUIT, SIG_IGN);
		g_signal = 0;
	}
	else if (g_signal == 1)
	{
		signal(SIGINT, sig_handler_exec);
		signal(SIGQUIT, sig_handler_exec);
	}
	signal(SIGTSTP, SIG_IGN);
}

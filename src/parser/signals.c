/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:22:29 by mimi-notebo       #+#    #+#             */
/*   Updated: 2025/05/13 16:24:19 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ctrl_c(void)
{
/*     if (g_signal == 0)
    {
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_signal = 1;
    }
    else if (g_signal == 1)
        write(1, "\n", 1); */
}

void	setup_signals(t_msh *msh)
{
	(void)msh;
	// struct sigaction	act;

	// act.sa_handler = &sig_handler;
	// act.sa_flags = SA_RESTART;
	// sigemptyset(&act.sa_mask);
	// if (sigaction(SIGINT, &act, NULL) == -1)
	// 	error_msh("Error: sigaction", msh, 1);
	// if (g_signal == 0)
	// {
	// 	act.sa_handler = SIG_IGN;
	// 	if (sigaction(SIGQUIT, &act, NULL) == -1)
	// 		error_msh("Error: sigaction", msh, 1);
	// }
	// else if (g_signal == 1)
	// {
	// 	act.sa_handler = &sig_handler;
	// 	if (sigaction(SIGQUIT, &act, NULL) == -1)
	// 		error_msh("Error: sigaction", msh, 1);
	// }
	// act.sa_handler = SIG_IGN;
	// if (sigaction(SIGTSTP, &act, NULL) == -1)
  	//   error_msh("Error: sigaction", msh, 1);
}

// static void sig_handler(int sig)
// {
// 	(void)sig;
//     // if (sig == SIGINT && (g_signal == 0 || g_signal == 1))
//     //     ctrl_c();
//     // if (sig == SIGQUIT && g_signal == 1)
//     //     ft_putendl_fd("Quit (core dumped)", 2);
// }

// static void ctrl_d(void)
// {
//     ft_putstr_fd("exit\n", 1);
// }
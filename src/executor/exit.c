/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:48:38 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/23 01:18:50 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_and_exit_ex(t_msh *msh)
{
    free_msh(msh);
    if (msh->env)
        free_env(msh->env);
    
    clear_history();
    
    if (rl_line_buffer)
        rl_free(rl_line_buffer);
    
    rl_attempted_completion_function = NULL;
    rl_completion_entry_function = NULL;
    
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    
    if (msh->cmd)
    {
        t_cmd *current = msh->cmd;
        while (current)
        {
            if (current->fd_in > 2)
                close(current->fd_in);
            if (current->fd_out > 2)
                close(current->fd_out);
            current = current->next;
        }
    }
}

void ft_exit(t_msh *msh, t_cmd *cmd)
{
    int exit_code = 0;
    
    // Reset signal flags
    g_sigint_received = 0;
    
    if (cmd->argv && cmd->argv[1])
    {
        char *endptr;
        long code = strtol(cmd->argv[1], &endptr, 10);
        
        if (*endptr != '\0')
        {
            ft_putstr_fd("exit: numeric argument required\n", 2);
            exit_code = 2;
        }
        else if (cmd->argv[2])
        {
            ft_putstr_fd("exit: too many arguments\n", 2);
            msh->exit_code = 1;
            return;
        }
        else
        {
            exit_code = (int)(code % 256);
        }
    }
    
    ft_putstr_fd("exit\n", 1);
    
    // Set exit flags instead of directly exiting
    msh->exit_requested = 1;
    msh->exit_code = exit_code;
    
    return; // Return to let the main loop handle the exit
}

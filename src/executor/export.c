/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:49:05 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/22 20:53:24 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_valid_identifier(const char *str, size_t len) {
    size_t i = 0;
    while (i < len) {
        if (!ft_isalnum(str[i]) && str[i] != '_') {
            return 0;
        }
        i++;
    }
    return 1;
}

static void handle_invalid_identifier(t_msh *msh, char *arg, char *equal_sign) {
    if ((!ft_isalpha(arg[0]) && arg[0] != '_') ||
        (equal_sign && equal_sign == arg) ||
        (equal_sign && !is_valid_identifier(arg, equal_sign - arg))) {
        ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
        msh->state = 1;
    }
}

static void process_argument(t_msh *msh, char *arg) {
    char *equal_sign = ft_strchr(arg, '=');

    handle_invalid_identifier(msh, arg, equal_sign);

    if (equal_sign) {
        *equal_sign = '\0';
        set_env(msh, arg, equal_sign + 1);
        *equal_sign = '=';
    } else {
        if (!get_env_type(msh, arg)) {
            add_env(msh, arg, NULL);
        }
    }
}

void ft_export(t_msh *msh, t_cmd *cmd) {
    int i = 1;

    if (!cmd->argv[1]) {
        print_export(msh->env, cmd->fd_out);
        return;
    }

    while (cmd->argv[i]) {
        process_argument(msh, cmd->argv[i]);
        i++;
    }
}

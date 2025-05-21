/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:49:05 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/21 17:45:23 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Forward declaration of helper function
int is_valid_identifier(const char *str, size_t len);

void ft_export(t_msh *msh, t_cmd *cmd) {
    int i = 1;

    if (!cmd->argv[1]) {
        print_export(msh->env, cmd->fd_out);
        return;
    }

    while (cmd->argv[i]) {
        char *arg = cmd->argv[i];
        char *equal_sign = ft_strchr(arg, '=');

        // Validate identifier
        if ((!ft_isalpha(arg[0]) && arg[0] != '_') ||
            (equal_sign && equal_sign == arg) ||
            (equal_sign && !is_valid_identifier(arg, equal_sign - arg))) {
            ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
            msh->state = 1;
        } else if (equal_sign) {
            *equal_sign = '\0';
            set_env(msh, arg, equal_sign + 1);
            *equal_sign = '=';
        } else {
            if (!get_env_type(msh, arg)) {
                add_env(msh, arg, NULL);
            }
        }
        i++;
    }
}

// Helper function to validate identifier
int is_valid_identifier(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (!ft_isalnum(str[i]) && str[i] != '_') {
            return 0;
        }
    }
    return 1;
}

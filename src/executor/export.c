/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:49:05 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/23 18:44:06 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	new_var_value(t_msh *msh, char *str)
{
	char	*var;
	char	*content;
	int		i;
	int		start;

	i = 0;
	while (str[i] != '=')
		i++;
	var = ft_substr(str, 0, i);
	start = ++i;
	while (str[i])
		i++;
	content = ft_substr(str, start, (i - start));
	if (!var || !content)
		return ;
	set_env(msh, var, content);
	free(var);
	free(content);
}

static void	add_var_value(t_msh *msh, char *str)
{
	char	*var;
	char	*new_content;
	char	*content;
	int		start;
	int		i;

	i = 0;
	while (str[i] != '=')
		i++;
	var = ft_substr(str, 0, (i - 1));
	start = ++i;
	while (str[i])
		i++;
	content = ft_substr(str, start, (i - start));
	if (!var || !content)
		return ;
	if (get_env(msh, var))
		new_content = ft_strjoin(get_env(msh, var), content);
	else
		new_content = ft_strjoin("", content);
	set_env(msh, var, new_content);
	return (free(var), free(content), free(new_content));
}

static void	add_vars(t_msh *msh, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+')
		{
			add_var_value(msh, str);
			break ;
		}
		if (str[i] == '=')
		{
			new_var_value(msh, str);
			break ;
		}
		i++;
	}
}

static int validate_export_char(char c) {
    return ft_isalnum(c) || c == '_';
}

static int parse_export(t_msh *msh, char *line) {
    int i;

    if (!ft_isalpha(line[0]) && line[0] != '_') {
        write(2, "Error: Invalid start character\n", 30);
        return error_msh(EXPORT, msh, 1), 0;
    }

    i = 0;
    while (line[i] && line[i] != '=') {
        if (!validate_export_char(line[i])) {
            write(2, "Error: Invalid character in variable name\n", 40);
            return error_msh(EXPORT, msh, 1), 0;
        }
        i++;
    }
    return 1;
}

void	ft_export(t_msh *msh, t_cmd *cmd)
{
	int	i;
	
	if (!cmd->argv[1]) 
	{
		print_export(msh->env, cmd->fd_out);
		return;
	}
	
	i = 1;
	while (cmd->argv[i]) 
	{
		if (!parse_export(msh, cmd->argv[i])) 
		{
			msh->state = 1;
			i++;
			continue;
		}
		if (ft_strchr(cmd->argv[i], '=')) 
		{
			add_vars(msh, cmd->argv[i]);
		} 
		else 
		{
			if (!get_env_type(msh, cmd->argv[i])) 
			{
				add_env(msh, cmd->argv[i], NULL);
			}
		}
		i++;
	}
}

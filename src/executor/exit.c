/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:55:29 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:26:56 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	all_nbr(char *line)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	while (line[i])
	{
		if (ft_isdigit(line[i]) == 0)
		{
			if (check == 1)
				return (false);
			if (line[i] == '-' || line[i] == '+')
				check = 1;
			else
				return (false);
		}
		i++;
	}
	return (true);
}

void	free_and_exit_ex(t_msh *msh)
{
	free_env(msh->env);
	free_msh(msh);
}

static bool	ft_atolli(const char *str, long long int *result)
{
	int				i;
	int				sign;
	long long int	prev_result;

	i = 0;
	*result = 0;
	sign = 1;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f'
		|| str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign *= -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		prev_result = *result;
		*result = (*result * 10) + (str[i] - '0');
		if ((*result / 10) != prev_result || (sign == 1 && *result < 0)
			|| (sign == -1 && *result > 0))
			return (false);
		i++;
	}
	*result *= sign;
	return (true);
}

static void	handle_exit_error(t_msh *msh, t_cmd *cmd)
{
	ft_putendl_fd("exit", 2);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(cmd->argv[1], 2);
	ft_putendl_fd(": numeric argument required", 2);
	free_and_exit_ex(msh);
	exit(2);
}

void	exit_extra_options(t_msh *msh, t_cmd *cmd)
{
	long long int	value;

	if (all_nbr(cmd->argv[1]) == true)
	{
		if (ft_atolli(cmd->argv[1], &value) == true)
		{
			value = value % 256;
			ft_putendl_fd("exit", 2);
			free_and_exit_ex(msh);
			exit(value);
		}
	}
	handle_exit_error(msh, cmd);
}

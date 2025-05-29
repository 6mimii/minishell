/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:55:26 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:24:49 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	env_lstsize(t_env *lst)
{
	t_env	*new;
	int		size;

	new = lst;
	size = 0;
	while (new != NULL)
	{
		size++;
		new = new->next;
	}
	return (size);
}

static int	get_var_index(t_env *list, t_env *tmp)
{
	t_env	*aux;
	int		index;

	aux = list;
	index = 0;
	while (aux)
	{
		if (ft_strncmp(tmp->type, aux->type, ft_strlen(tmp->type) + 1) > 0)
			index++;
		aux = aux->next;
	}
	return (index);
}

static void	print_var(t_env *aux, char *content, int fd)
{
	ft_putstr_fd("declare -x ", fd);
	ft_putstr_fd(aux->type, fd);
	if (content)
	{
		ft_putstr_fd("=\"", fd);
		ft_putstr_fd(aux->content, fd);
		write(fd, "\"", 1);
	}
	ft_putstr_fd("\n", fd);
}

void	print_export(t_env *env, int fd)
{
	t_env	*aux;
	int		index;

	aux = env;
	index = 0;
	while (aux)
	{
		aux->index = get_var_index(env, aux);
		aux = aux->next;
	}
	while (index < env_lstsize(env))
	{
		aux = env;
		while (index != aux->index)
			aux = aux->next;
		print_var(aux, aux->content, fd);
		index++;
	}
}

void	ft_env(t_msh *msh, t_cmd *cmd, char *next)
{
	t_env	*tmp;

	tmp = msh->env;
	if (!tmp)
		return ;
	if (next)
		return (print_env_error(next, msh));
	while (tmp)
	{
		if (tmp->content)
		{
			ft_putstr_fd(tmp->type, cmd->fd_out);
			ft_putstr_fd("=", cmd->fd_out);
			ft_putendl_fd(tmp->content, cmd->fd_out);
		}
		tmp = tmp->next;
	}
	msh->state = 0;
}

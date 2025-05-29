/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:55:37 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:24:37 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_env(t_msh *msh, char *var, char *content)
{
	t_env	*aux;

	aux = msh->env;
	while (aux->next)
		aux = aux->next;
	aux->next = malloc(sizeof(t_env));
	if (!aux->next)
		return ;
	aux = aux->next;
	aux->type = ft_strdup(var);
	if (content)
		aux->content = ft_strdup(content);
	else
		aux->content = NULL;
	aux->next = NULL;
}

void	set_env(t_msh *msh, char *var, char *new)
{
	t_env	*aux;

	aux = msh->env;
	if (!new)
		return ;
	while (aux)
	{
		if (ft_strncmp(var, aux->type, ft_strlen(var) + 1) == 0)
		{
			free(aux->content);
			aux->content = ft_strdup(new);
			return ;
		}
		aux = aux->next;
	}
	add_env(msh, var, new);
}

char	*get_env(t_msh *msh, char *var)
{
	t_env	*aux;

	aux = msh->env;
	while (aux)
	{
		if (ft_strncmp(var, aux->type, ft_strlen(var) + 1) == 0)
			return (aux->content);
		aux = aux->next;
	}
	return (NULL);
}

char	*get_env_type(t_msh *msh, char *var)
{
	t_env	*aux;

	aux = msh->env;
	while (aux)
	{
		if (ft_strncmp(var, aux->type, ft_strlen(var) + 1) == 0)
			return (aux->type);
		aux = aux->next;
	}
	return (NULL);
}

void	print_env_error(char *next, t_msh *msh)
{
	ft_putstr_fd("env: '", 2);
	ft_putstr_fd(next, 2);
	ft_putendl_fd("': No such file or directory", 2);
	msh->state = 127;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_b.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:31:45 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:35:27 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special_var(char *var)
{
	return (var[0] == '?' && !var[1]);
}

char	*handle_special_var(char *var, t_msh *msh)
{
	(void)var;
	return (ft_itoa(msh->state));
}

char	*process_expand_var(char *var, t_msh *msh, char *line)
{
	t_env	*aux;
	int		i;

	if (msh->unset_vars)
	{
		i = 0;
		while (msh->unset_vars[i])
		{
			if (ft_strcmp(var, msh->unset_vars[i]) == 0)
				return (ft_strdup(""));
			i++;
		}
	}
	aux = msh->env;
	while (aux)
	{
		if (ft_strcmp(var, aux->type) == 0)
		{
			free(line);
			line = ft_strdup(aux->content);
			break ;
		}
		aux = aux->next;
	}
	return (line);
}

void	free_env_node(t_env *env)
{
	if (!env)
		return ;
	if (env->type)
		free(env->type);
	if (env->content)
		free(env->content);
	free(env);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils_c.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:05:32 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:06:38 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_envp_array(t_msh *msh)
{
	char	**new_envp;
	char	**old_envp;

	new_envp = create_envp_array(msh);
	if (!new_envp)
		return (0);
	old_envp = msh->envp;
	msh->envp = new_envp;
	if (old_envp != msh->original_envp)
		cleanup_envp_array(old_envp, -1);
	return (1);
}

void	add_to_unset_list(t_msh *msh, char *var)
{
	int		len;
	char	**new_list;
	int		i;

	len = get_unset_vars_length(msh, var);
	if (len == -1)
		return ;
	new_list = create_new_unset_list(msh, var, len);
	if (!new_list)
		return ;
	if (msh->unset_vars)
	{
		i = 0;
		while (msh->unset_vars[i])
			free(msh->unset_vars[i++]);
		free(msh->unset_vars);
	}
	msh->unset_vars = new_list;
}

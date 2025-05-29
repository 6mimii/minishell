/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:55:11 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 15:03:37 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_msh *msh, t_cmd *cmd)
{
	int	changed;

	if (!cmd || !cmd->argv || !cmd->argv[1])
	{
		msh->state = 0;
		return ;
	}
	changed = 0;
	process_unset_args(msh, cmd, &changed);
	if (changed && !update_envp_array(msh))
		msh->state = 1;
	else
		msh->state = 0;
}

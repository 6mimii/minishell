/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:50:08 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/27 17:46:28 by mdoudi-b         ###   ########.fr       */
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
	msh->state = (changed && !update_envp_array(msh)) ? 1 : 0;
}

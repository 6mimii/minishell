/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:18 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/27 17:34:29 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		free(matrix[i++]);
	free(matrix);
	matrix = NULL;
}

void	free_commands_helper(t_cmd **cmd, t_cmd *aux)
{
	if ((*cmd)->fd_in > 2)
	{
		if (access(".here_doc.tmp", F_OK) == 0)
			unlink(".here_doc.tmp");
		close((*cmd)->fd_in);
	}
	if ((*cmd)->fd_out > 2)
		close((*cmd)->fd_out);
	free_matrix((*cmd)->argv);
	free(*cmd);
	*cmd = aux;
}

void	free_commands(t_cmd **cmd)
{
	t_cmd	*aux;

	if (!cmd || !*cmd)
		return ;
	while (*cmd)
	{
		aux = (*cmd)->next;
		free_commands_helper(cmd, aux);
	}
	*cmd = NULL;
}




/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:56:25 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/29 14:56:26 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*exp_line_helper(char *str, int *i)
{
	if (str[*i] == '\\')
		return (no_expand_var(str, i));
	else if (str[*i] == '$' && str[*i + 1] == '~')
	{
		*i += 2;
		return (ft_strdup("$~"));
	}
	else if (str[*i] == '$')
		return (get_exp(str, i, NULL));
	return (get_word(str, i));
}

char	*exp_line(char *str, t_msh *msh)
{
	char	*aux;
	char	*line;
	int		i;

	(void)msh;
	line = ft_strdup("");
	i = 0;
	while (str[i])
	{
		aux = exp_line_helper(str, &i);
		line = ft_strjoin(line, aux);
	}
	free(str);
	str = ft_strdup(line);
	free(line);
	return (str);
}

char	*expand_heredoc(char *line, t_msh *msh)
{
	int		i;
	char	*aux;

	i = 0;
	aux = ft_strdup(line);
	free(line);
	while (aux[i])
	{
		if (aux[i] == '$')
			return (exp_line(aux, msh));
		i++;
	}
	return (aux);
}

void	ctrl_c_hd(int signal)
{
	(void)signal;
	ft_putstr_fd("\n", 1);
	exit(130);
}

void	free_and_exit_hd(t_msh *msh, t_cmd *new, int state)
{
	(void)new;
	free_and_exit("", msh, state, false);
}

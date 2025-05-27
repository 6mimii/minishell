/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:04:25 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/27 17:33:09 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	expand_flag(t_token *tok)
{
	t_token	*aux;

	if (!tok)
		return ;
	aux = tok;
	while (aux)
	{
		if (!aux->content)
		{
			aux = aux->next;
			continue ;
		}
		aux->exp = 0;
		if (aux->type == T_WORD || aux->type == T_DQ)
		{
			if (check_dollar(aux->content) == 1)
				aux->exp = 1;
			else if (check_home(aux->content) == 1)
				aux->exp = 2;
			else if (check_home(aux->content) == 2)
				aux->exp = 3;
		}
		aux = aux->next;
	}
}

int	check_home(const char *str)
{
	if (!str || !*str)
		return (0);
	if (str[0] == '~' && (!str[1] || str[1] == '/'))
		return (1);
	else if (str[0] == '~' && str[1] == '/')
		return (2);
	return (0);
}

void	expand_both(t_token *tok, t_msh *msh)
{
	char	*home;
	char	*aux;

	if (!tok || !tok->content || !msh)
		return ;
	aux = ft_strdup(&tok->content[1]);
	if (!aux)
		return ;
	expand_home(tok, msh);
	home = ft_strdup(tok->content);
	if (!home)
		return ;
	free(tok->content);
	tok->content = ft_strjoin(home, aux);
	if (!tok->content)
		return ;
	free(home);
	free(aux);
}

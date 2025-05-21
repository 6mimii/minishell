/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+    */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:08:15 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/21 00:12:13 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_word(char *s1, int *i)
{
	char	*line;
	int		len;
	int		j;

	len = 0;
	j = *i;
	while (s1[j] && s1[j] != '$' && s1[j] != '\\')
	{
		j++;
		len++;
	}
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	j = 0;
	while (s1[*i] && s1[*i] != '$' && s1[*i] != '\\')
	{
		line[j++] = s1[*i];
		*i += 1;
	}
	line[j] = '\0';
	return (line);
}

char	*get_exp(char *line, int *i, t_msh *msh)
{
	char	*ret;
	char	*var;
	int		start;
	int		j;

	start = *i + 1;
	*i += 1;
	if (line[start] == '{')
	{
		start++;
		while (line[*i] && line[*i] != '}')
			*i += 1;
		if (!line[*i])
			start--;
	}
	else
		while (ft_isalnum(line[*i]) || line[*i] == '_')
			*i += 1;
	if (*i == start && line[*i] == '?')
		*i += 1;
	var = ft_substr(line, start, (*i - start));
	if (line[*i] == '}')
		*i += 1;
	
	if (msh->unset_vars)
	{
		j = 0;
		while (msh->unset_vars[j])
		{
			if (ft_strcmp(var, msh->unset_vars[j]) == 0)
			{
				free(var);
				return (ft_strdup(""));
			}
			j++;
		}
	}
	
	ret = expand_var(var, msh, 0);
	free(var);
	return (ret);
}

char	*expand_var(char *var, t_msh *msh, int len)
{
	t_env	*aux;
	char	*line;
	int     i;

	line = ft_strdup("");
	
	if (var[0] == '?' && !var[1])
	{
		free(line);
		line = ft_itoa(msh->state);
		return (line);
	}
	
	if (msh->unset_vars)
	{
		i = 0;
		while (msh->unset_vars[i])
		{
			if (ft_strcmp(var, msh->unset_vars[i]) == 0)
			{
				return (line);
			}
			i++;
		}
	}
	
	aux = msh->env;
	while (aux)
	{
		if (ft_strcmp(var, aux->type) == 0)
		{
			if (aux->content)
			{
				free(line);
				line = ft_strdup(aux->content);
			}
			break;
		}
		aux = aux->next;
	}
	
	(void)len;
	
	return (line);
}

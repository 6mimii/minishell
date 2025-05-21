/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:08:15 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/21 17:45:50 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_word(char *s1, int *i)
{
	char	*line;
	int		len;
	int		j;

	if (!s1 || !i)
		return (ft_strdup(""));
	len = 0;
	j = *i;
	while (s1[j] && s1[j] != '$' && s1[j] != '\\')
	{
		j++;
		len++;
	}
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
		return (ft_strdup(""));
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

	if (!line || !i || !msh)
		return (ft_strdup(""));
	start = *i + 1;
	*i += 1;
	if (!line[*i] || line[*i] == ' ' || line[*i] == '"' || line[*i] == '\'')
		return (ft_strdup("$"));
	if (line[start] == '{')
	{
		start++;
		while (line[*i] && line[*i] != '}')
			*i += 1;
		if (!line[*i])
			start--;
	}
	else
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			*i += 1;
	if (*i == start && line[*i] == '?')
		*i += 1;
	var = ft_substr(line, start, (*i - start));
	if (!var)
		return (ft_strdup(""));
	if (line[*i] == '}')
		*i += 1;
	ret = expand_var(var, msh, 0);
	free(var);
	return (ret);
}

char	*expand_var(char *var, t_msh *msh, int len)
{
	t_env	*aux;
	char	*line;

	if (!var || !msh)
		return (ft_strdup(""));
	aux = msh->env;
	line = ft_strdup("");
	if (!line)
		return (ft_strdup(""));
	if (!var || var[0] == '\0')
		return (line);
	if (var[0] == '?' && !var[1])
	{
		free(line);
		line = ft_itoa(msh->state);
		return (line ? line : ft_strdup(""));
	}
	while (aux)
	{
		len = ft_strlen(aux->type);
		if (ft_strncmp(var, aux->type, len) == 0 && var[len] == '\0')
		{
			if (aux->content)
			{
				free(line);
				return (ft_strdup(aux->content));
			}
			return (line);
		}
		aux = aux->next;
	}
	return (line);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:08:15 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/20 01:27:04 by mimi-notebo      ###   ########.fr       */
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
	ret = expand_var(var, msh, 0);
	write(2, "Debug: Extracted variable: ", 26);
	write(2, var, ft_strlen(var));
	write(2, "\n", 1);
	write(2, "Debug: Variable value: ", 23);
	write(2, ret, ft_strlen(ret));
	write(2, "\n", 1);
	free(var);
	return (ret);
}

char	*expand_var(char *var, t_msh *msh, int len)
{
	t_env	*aux;
	char	*line;

	aux = msh->env;
	line = ft_strdup("");
	if (var[0] == '?' && !var[1])
	{
		free(line);
		line = ft_itoa(msh->state);
		return (line);
	}
	while (aux)
	{
		len = ft_strlen(aux->type);
		if (ft_strncmp(var, aux->type, len) == 0 && var[len] == '\0')
		{
			if (aux->content)
			{
				free(line);
				line = ft_strdup(aux->content);
			}
		}
		aux = aux->next;
	}
	return (line);
}

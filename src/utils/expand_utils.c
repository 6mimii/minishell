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

static int	is_special_var(char *var);
static char	*handle_special_var(char *var, t_msh *msh);
static char	*process_expand_var(char *var, t_msh *msh, char *line);

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

static void	parse_variable_name(char *line, int *i, int *start)
{
	*start = *i + 1;
	*i += 1;
	if (line[*start] == '{')
	{
		(*start)++;
		while (line[*i] && line[*i] != '}')
			*i += 1;
		if (!line[*i])
			(*start)--;
	}
	else
		while (ft_isalnum(line[*i]) || line[*i] == '_')
			*i += 1;
	if (*i == *start && line[*i] == '?')
		*i += 1;
}

static int	check_unset_var(t_msh *msh, char *var)
{
	int	j;

	if (!msh->unset_vars)
		return (0);
	j = 0;
	while (msh->unset_vars[j])
	{
		if (ft_strcmp(var, msh->unset_vars[j]) == 0)
			return (1);
		j++;
	}
	return (0);
}

char	*get_exp(char *line, int *i, t_msh *msh)
{
	char	*ret;
	char	*var;
	int		start;

	parse_variable_name(line, i, &start);
	var = ft_substr(line, start, (*i - start));
	if (line[*i] == '}')
		*i += 1;
	if (check_unset_var(msh, var))
	{
		free(var);
		return (ft_strdup(""));
	}
	ret = expand_var(var, msh, 0);
	free(var);
	return (ret);
}

char	*expand_var(char *var, t_msh *msh, int len)
{
	char	*line;

	(void)len;
	line = ft_strdup("");
	if (is_special_var(var))
	{
		free(line);
		return (handle_special_var(var, msh));
	}
	return (process_expand_var(var, msh, line));
}

static int	is_special_var(char *var)
{
	return (var[0] == '?' && !var[1]);
}

static char	*handle_special_var(char *var, t_msh *msh)
{
	(void)var;
	return (ft_itoa(msh->state));
}

static char	*process_expand_var(char *var, t_msh *msh, char *line)
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

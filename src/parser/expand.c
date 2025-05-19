/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:42:47 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/20 01:27:04 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*no_expand_var(char *s1, int *i)
{
	char	*line;
	int		len;
	int		j;

	len = 1;
	*i += 1;
	j = *i + 1;
	while (s1[j] && s1[j] != '$' && s1[j] != '\\')
	{
		j++;
		len++;
	}
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	j = 0;
	line[j++] = s1[*i];
	*i += 1;
	while (s1[*i] && s1[*i] != '$' && s1[*i] != '\\')
	{
		line[j++] = s1[*i];
		*i += 1;
	}
	line[j] = '\0';
	return (line);
}

void	expand_content(t_token *tok, t_msh *msh)
{
	int		i;
	char	*line;
	char	*aux;

	if (!tok || !tok->content || !msh)
		return;

	i = 0;
	line = ft_strdup("");
	while (tok->content[i])
	{
		if (tok->content[i] == '\\')
			aux = no_expand_var(tok->content, &i);
		else if (tok->content[i] == '$' && tok->content[i + 1] == '~')
		{
			aux = ft_strdup("$~");
			i += 2;
		}
		else if (tok->content[i] == '$')
			aux = get_exp(tok->content, &i, msh);
		else
			aux = get_word(tok->content, &i);
		line = ft_strjoin(line, aux);
	}
	free(tok->content);
	tok->content = ft_strdup(line);
	free(line);
}

static void	expand_home(t_token *tok, t_msh *msh)
{
	char	*line;
	t_env	*aux;
	
	if (!tok || !msh)
		return;

	line = NULL;
	
	if (!msh->env)
		return;
	
	aux = msh->env;
	while (aux)
	{
		if (ft_strncmp("HOME", aux->type, 5) == 0)
		{
			line = ft_strdup(aux->content);
			break ;
		}
		aux = aux->next;
	}
	if (!line)
		line = ft_strdup("");
	free(tok->content);
	tok->content = ft_strdup(line);
	free(line);
}

static void	expand_both(t_token *tok, t_msh *msh)
{
	char	*home;
	char	*aux;
	
	if (!tok || !tok->content || !msh)
		return;
	
	aux = ft_strdup(&tok->content[1]);
	if (!aux)
		return;
	expand_home(tok, msh);
	home = ft_strdup(tok->content);
	if (!home)
		return;
	free(tok->content);
	tok->content = ft_strjoin(home, aux);
	if (!tok->content)
		return;
	free(home);
	free(aux);
}

void	expand_tokens(t_token **tokens, t_msh *msh)
{
	t_token	*tmp;
	
	if (!tokens || !*tokens || !msh)
		return;

	tmp = *tokens;
	while (tmp)
	{
		if (!tmp->content)
		{
			tmp = tmp->next;
			continue;
		}
		
		if (tmp->type == T_DL)
			tmp = tmp->next;
		else if (tmp->exp == 1)
			expand_content(tmp, msh);
		else if (tmp->exp == 2)
			expand_home(tmp, msh);
		else if (tmp->exp == 3)
			expand_both(tmp, msh);
		if (tmp)
			tmp = tmp->next;
	}
}

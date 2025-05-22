/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:42:47 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/23 01:30:21 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*no_expand_var(char *s1, int *i)
{
	char	*line;
	int		len;
	int		j;

	if (!s1 || !i)
		return (ft_strdup(""));

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
		return (ft_strdup(""));
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

static char	*process_token_character(char *token_content, int *i, t_msh *msh)
{
	char	*aux;

	if (token_content[*i] == '\\')
		aux = no_expand_var(token_content, i);
	else if (token_content[*i] == '$' && token_content[*i + 1] == '~')
	{
		aux = ft_strdup("$~");
		*i += 2;
	}
	else if (token_content[*i] == '$')
		aux = get_exp(token_content, i, msh);
	else
		aux = get_word(token_content, i);
		
	return (aux);
}

static char	*build_expanded_content(char *token_content, t_msh *msh)
{
	int		i;
	char	*line;
	char	*aux;
	char	*temp;

	i = 0;
	line = ft_strdup("");
	if (!line)
		return (NULL);
		
	while (token_content[i])
	{
		aux = process_token_character(token_content, &i, msh);
		if (!aux)
		{
			free(line);
			return (NULL);
		}
		
		temp = line;
		line = ft_strjoin(line, aux);
		free(temp);
		free(aux);
		
		if (!line)
			return (NULL);
	}
	
	return (line);
}

void	expand_content(t_token *tok, t_msh *msh)
{
	char	*expanded;

	if (!tok || !tok->content || !msh)
		return;

	expanded = build_expanded_content(tok->content, msh);
	if (!expanded)
		return;
	
	free(tok->content);
	tok->content = expanded;
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
		
	if (tok->content)
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
	{
		free(aux);
		return;
	}
	
	if (tok->content)
		free(tok->content);
		
	tok->content = ft_strjoin(home, aux);
	
	free(home);
	free(aux);
}

static void	process_token_expansion(t_token *tmp, t_msh *msh)
{
	if (!tmp->content)
		return;
		
	if (tmp->type == T_DL)
		return;
	else if (tmp->exp == 1)
		expand_content(tmp, msh);
	else if (tmp->exp == 2)
		expand_home(tmp, msh);
	else if (tmp->exp == 3)
		expand_both(tmp, msh);
}

void	expand_tokens(t_token **tokens, t_msh *msh)
{
	t_token	*tmp;
	
	if (!tokens || !*tokens || !msh)
		return;

	tmp = *tokens;
	while (tmp)
	{
		process_token_expansion(tmp, msh);
		
		if (tmp && tmp->type == T_DL)
			tmp = tmp->next;
			
		if (tmp)
			tmp = tmp->next;
	}
}

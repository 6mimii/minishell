/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:42:47 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/05/18 20:16:11 by mdoudi-b         ###   ########.fr       */
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

	// Safety checks
	if (!tok || !tok->content || !msh) {
		write(2, "Invalid parameters in expand_content\n", 37);
		return;
	}

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
	
	// Safety checks
	if (!tok || !msh) {
		write(2, "Invalid parameters in expand_home\n", 34);
		return;
	}

	line = NULL;
	
	if (!msh->env) {
		write(2, "Warning: msh->env is NULL in expand_home\n", 41);
		return;
	}
	
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
	
	// Safety checks
	if (!tok || !tok->content || !msh) {
		write(2, "Invalid parameters in expand_both\n", 34);
		return;
	}
	
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

void	expand_tokens(t_token **tokens, t_msh *msh)
{
	t_token	*tmp;

	// Debug and safety check
	write(2, "In expand_tokens\n", 17);
	
	if (!tokens || !*tokens) {
		write(2, "No tokens to expand\n", 20);
		return;
	}
	
	if (!msh) {
		write(2, "Warning: msh is NULL in expand_tokens\n", 38);
		return;
	}

	tmp = *tokens;
	while (tmp)
	{
		if (!tmp->content) {
			write(2, "Warning: Token with NULL content in expand_tokens\n", 50);
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

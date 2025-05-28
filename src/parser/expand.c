#include "../minishell.h"

static char	*process_expand_content(char *content, int *i, t_msh *msh);

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
	char	*temp;

	if (!tok || !tok->content || !msh)
		return ;
	i = 0;
	line = ft_strdup("");
	while (tok->content[i])
	{
		aux = process_expand_content(tok->content, &i, msh);
		temp = line;
		line = ft_strjoin(line, aux);
		free(temp);
		free(aux);
	}
	free(tok->content);
	tok->content = line;
}

static char	*process_expand_content(char *content, int *i, t_msh *msh)
{
	char	*aux;

	if (content[*i] == '\\')
		aux = no_expand_var(content, i);
	else if (content[*i] == '$' && content[*i + 1] == '~')
	{
		aux = ft_strdup("$~");
		*i += 2;
	}
	else if (content[*i] == '$' && content[*i + 1] && (ft_isalnum(content[*i
					+ 1]) || content[*i + 1] == '_' || content[*i + 1] == '?'
			|| content[*i + 1] == '{'))
	{
		aux = get_exp(content, i, msh);
	}
	else if (content[*i] == '$')
	{
		aux = ft_strdup("$");
		*i += 1;
	}
	else
		aux = get_word(content, i);
	return (aux);
}

void	expand_home(t_token *tok, t_msh *msh)
{
	char	*line;
	t_env	*aux;

	if (!tok || !msh)
		return ;
	line = NULL;
	if (!msh->env)
		return ;
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


void	expand_tokens(t_token **tokens, t_msh *msh)
{
	t_token	*tmp;

	if (!tokens || !*tokens || !msh)
		return ;
	tmp = *tokens;
	while (tmp)
	{
		if (!tmp->content)
		{
			tmp = tmp->next;
			continue ;
		}
		if (tmp->type == T_DL)
			tmp = tmp->next;
		else if (tmp->type == T_DQ && check_dollar(tmp->content) == 1)
			expand_content(tmp, msh);
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

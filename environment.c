#include "minishell.h"

static void init_env_lst(t_env **lst, char  **envp) // lista donde guardaremos las v. entorno, variables d entorno
{
	t_env	*new;
	int		i;
	int		x;
	int		j;

	i = -1;
	while(envp[++i])
	{
		x = 0;
		new = malloc(sizeof(t_env));
		if (!new)
			return ;
		while (envp[i][x] != '=')
			x++;
		new->type = ft_substr(envp[i], 0, x);
		j = ++x;
		while(envp[i][j])
			j++;
		new->content = ft_substr(envp[i], x, (j - x));
		new->next = NULL;
		add_back((t_token **)lst, (t_token *)new);
	}
}

t_env	*enviroment_lst(char **envp)
{
	t_env	*env;
	
	if (!envp || !*envp)
		return (NULL);
	env = NULL;
	init_env_lst(&env, envp);
	return (env);
}
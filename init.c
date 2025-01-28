#include "minishell.h"

void init_msh(char **envp, t_msh *msh)
{
	msh->env = enviroment_lst(envp);
	msh->parse_error = 10;
}
#include "minishell.h"

void init_msh(char **envp, t_msh *msh)
{
	msh->env = enviroment_lst(envp);
	msh->envp = envp;
}

void get_input(t_msh *msh)
{
	msh->input = readline("Mimishell% ");
	if (!msh->input)
		crtl_d();
	while (msh->input)
	{
		if (msh->input[0] == '\0')
			free(msh->input);
		else
		{
			add_history(msh->input);
			msh->tokens = set_tokens(msh->input, msh);
		}
		msh->input = readline("Mimishell% ");
		if(!msh->input)
			ctrl_d();
	}
	free_msh(msh);
}

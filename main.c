#include "minishell.h"

int main (int ac, char **av, char **envp)
{
	t_msh	msh;
	(void)ac;
	(void)av;
	init_msh(envp, &msh);
	get_input(&msh);
}

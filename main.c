#include "minishell.h"

int main (int ac, char **av, char **envp)
{
	t_msh	msh;
	(void)ac;
	(void)av;
	msh.parse_error = 0;
	init_msh(envp, &msh);
	printf("%d\n", msh.parse_error);
}

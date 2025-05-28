#include "../minishell.h"







void	ft_unset(t_msh *msh, t_cmd *cmd)
{
	int	changed;

	if (!cmd || !cmd->argv || !cmd->argv[1])
	{
		msh->state = 0;
		return ;
	}
	changed = 0;
	process_unset_args(msh, cmd, &changed);
	msh->state = (changed && !update_envp_array(msh)) ? 1 : 0;
}

#include "minishell.h"

void add_back(t_token **lst, t_token *new)
{
	t_token		*new_node;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}

	new_node = *lst;
	while (new_node->next != NULL)
		new_node = new_node->next;
	new_node->next = new;
}

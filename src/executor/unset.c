/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimi-notebook <mimi-notebook@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:50:08 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/20 01:27:04 by mimi-notebo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
** Libera la memoria de un nodo de entorno
*/
static void	free_env_node(t_env *env)
{
	if (!env)
		return;
	if (env->type)
		free(env->type);
	if (env->content)
		free(env->content);
	free(env);
}

/*
** Cuenta el número de nodos en la lista enlazada de entorno
*/
static int	count_env_nodes(t_env *env)
{
	t_env	*curr;
	int		count;

	count = 0;
	curr = env;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

/*
** Elimina una variable de entorno de la lista enlazada
** Devuelve 1 si se encontró y eliminó la variable, 0 si no
*/
static int	delete_env_var(t_msh *msh, char *var)
{
	t_env	*curr;
	t_env	*prev;

	if (!msh->env || !var)
		return (0);
	
	prev = NULL;
	curr = msh->env;
	
	// Caso especial: el primer nodo es el que buscamos
	if (curr && ft_strcmp(curr->type, var) == 0)
	{
		msh->env = curr->next;
		free_env_node(curr);
		return (1);
	}
	
	// Buscar en el resto de la lista
	while (curr)
	{
		if (ft_strcmp(curr->type, var) == 0)
		{
			prev->next = curr->next;
			free_env_node(curr);
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

/*
** Crea un nuevo array de strings para el entorno a partir de la lista enlazada
*/
static char	**create_envp_array(t_msh *msh)
{
	int		env_count;
	char	**new_envp;
	t_env	*curr;
	int		i;
	char	*tmp;
	
	env_count = count_env_nodes(msh->env);
	new_envp = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!new_envp)
		return (NULL);
	
	i = 0;
	curr = msh->env;
	while (curr)
	{
		// Formato: "type=content"
		if (curr->content)
		{
			tmp = ft_strjoin(curr->type, "=");
			if (!tmp)
			{
				while (--i >= 0)
					free(new_envp[i]);
				free(new_envp);
				return (NULL);
			}
			new_envp[i] = ft_strjoin(tmp, curr->content);
			free(tmp);
			if (!new_envp[i])
			{
				while (--i >= 0)
					free(new_envp[i]);
				free(new_envp);
				return (NULL);
			}
		}
		else
		{
			// Para variables sin contenido
			new_envp[i] = ft_strdup(curr->type);
		}
		i++;
		curr = curr->next;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

/*
** Actualiza el array de entorno después de modificar la lista enlazada
** Implementación mejorada para una gestión de memoria más segura
*/
static int	update_envp_array(t_msh *msh)
{
	char	**new_envp;

	// Primero creamos el nuevo array
	new_envp = create_envp_array(msh);
	if (!new_envp)
		return (0);

	// Usamos un enfoque más seguro:
	// 1. Primero liberar el array antiguo sin tocar los strings
	if (msh->envp)
	{
		// 2. Liberamos todos los strings antiguos
		int i = 0;
		while (msh->envp[i])
		{
			if (msh->envp[i])
				free(msh->envp[i]);
			i++;
		}
		// 3. Finalmente liberamos el array antiguo
		free(msh->envp);
	}

	// 4. Asignamos el nuevo array
	msh->envp = new_envp;
	
	return (1);
}

/*
** Implementación del comando unset
** Elimina una o más variables del entorno
*/
void	ft_unset(t_msh *msh, t_cmd *cmd)
{
	int	i;
	int	changed;

	if (!cmd || !cmd->argv || !cmd->argv[1])
	{
		msh->state = 0;
		return;
	}
	
	changed = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (delete_env_var(msh, cmd->argv[i]))
			changed = 1;
		i++;
	}
	
	// Solo actualizamos el array de entorno si se eliminó alguna variable
	if (changed)
	{
		if (!update_envp_array(msh))
			msh->state = 1; // Error al actualizar el entorno
		else
			msh->state = 0; // Sin errores
	}
	else
		msh->state = 0;
}

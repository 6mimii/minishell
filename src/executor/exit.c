/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:48:38 by fsaffiri          #+#    #+#             */
/*   Updated: 2025/05/21 17:45:23 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* 
* La funcionalidad principal de exit ha sido trasladada a executor.c
* para asegurar que funcione correctamente en todos los escenarios:
* - Modo interactivo
* - En scripts
* - En tuberías
*/

void	free_and_exit_ex(t_msh *msh)
{
	// Asegurarse de que todas las estructuras se limpian correctamente
	free_msh(msh);
	if (msh->env)
		free_env(msh->env);
}

void	ft_exit(t_msh *msh, t_cmd *cmd)
{
	// Esta función ya no se usa directamente
	// La implementación completa está en is_builtin en executor.c
	// Pero mantenemos la firma para compatibilidad
	(void)msh;
	(void)cmd;
}

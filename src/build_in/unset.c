/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:31:44 by mateon            #+#    #+#             */
/*   Updated: 2026/01/05 20:01:19 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	remove_from_struct(t_data *data, t_envp *current)
{
	t_envp	*to_delete;

	to_delete = current;
	if (to_delete->prev)
		to_delete->prev->next = to_delete->next;
	else
		data->env_var = to_delete->next;
	if (to_delete->next)
		to_delete->next->prev = to_delete->prev;
}

void	build_in_unset(t_data *data, char **args)
{
	t_envp	*current;
	int		i;

	i = 1;
	while (args[i])
	{
		current = data->env_var;
		while (current)
		{
			if (ft_strcmp(current->name, args[i]) == 0)
			{
				remove_from_struct(data, current);
				free(current->name);
				free(current->value);
				free(current);
				break ;
			}
			current = current->next;
		}
		i++;
	}
	data->exit_code = 0;
}

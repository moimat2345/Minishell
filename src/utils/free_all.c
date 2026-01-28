/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:28:22 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 02:36:09 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	free_envp(t_envp **env_var)
{
	t_envp	*current;
	t_envp	*next_var;

	if (!env_var || !*env_var)
		return ;
	current = *env_var;
	while (current)
	{
		next_var = current->next;
		if (current->name)
			free(current->name);
		if (current->value)
			free(current->value);
		free(current);
		current = next_var;
	}
	*env_var = NULL;
}

void	free_cmds(t_cmd **cmds)
{
	t_cmd	*current;
	t_cmd	*next_cmd;

	if (!cmds || !*cmds)
		return ;
	current = *cmds;
	while (current)
	{
		next_cmd = current->next;
		if (current->input_file)
			free(current->input_file);
		if (current->output_file)
			free(current->output_file);
		if (current->input_fd > 2)
			close(current->input_fd);
		if (current->output_fd > 2)
			close(current->output_fd);
		if (current->delimiter)
			free(current->delimiter);
		if (current->args)
			free_strs(current->args);
		free(current);
		current = next_cmd;
	}
	*cmds = NULL;
}

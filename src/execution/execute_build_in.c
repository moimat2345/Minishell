/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_build_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 16:50:02 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/14 12:05:53 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_build_in(t_cmd *cmd, t_data *data)
{
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		build_in_cd(data, cmd->args);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		build_in_export(data, cmd->args);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		build_in_unset(data, cmd->args);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		build_in_exit(data, cmd->args);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		build_in_pwd(data);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		build_in_echo(data, cmd->args);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		build_in_env(data, cmd->args);
}

void	found_built_in_commands(t_data *data)
{
	t_cmd	*current;

	current = data->cmds;
	while (current)
	{
		if (current->args && current->args[0])
		{
			if (ft_strcmp(current->args[0], "cd") == 0
				|| ft_strcmp(current->args[0], "echo") == 0
				|| ft_strcmp(current->args[0], "env") == 0
				|| ft_strcmp(current->args[0], "exit") == 0
				|| ft_strcmp(current->args[0], "export") == 0
				|| ft_strcmp(current->args[0], "pwd") == 0
				|| ft_strcmp(current->args[0], "unset") == 0)
				current->is_built_in = 1;
			else
				current->is_built_in = 0;
		}
		current = current->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 11:58:17 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 10:37:27 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_arg_error(char **args, t_tokens *token, int i)
{
	while (i > 0)
		free(args[--i]);
	free(args);
	exit_with_error("Memory allocation failed", 1, &(token->data));
}

void	exit_with_error(char *message, int exit_code, t_data **data)
{
	error_minishell(message, exit_code, data);
	if (data && *data)
	{
		if ((*data)->env_var)
			free_envp(&(*data)->env_var);
		cleanup_for_next_command(*data);
		if ((*data)->envp)
			free_strs((*data)->envp);
		free(*data);
	}
	exit(exit_code);
}

void	error_unset_path(char *cmd, t_data **data)
{
	error_minishell(cmd, 127, data);
	ft_putstr_fd(": No such file or directory\n", 2);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 02:12:58 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 00:19:25 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_new_cmd(t_cmd **new_cmd, t_data **data)
{
	t_cmd	*cmd;
	t_data	*data_local;

	data_local = *data;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit_with_error("Memory allocation failed", 1, &data_local);
	cmd->args = NULL;
	cmd->args_count = 0;
	cmd->is_built_in = 0;
	cmd->has_redir_error = 0;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->delimiter = NULL;
	cmd->input_fd = -1;
	cmd->output_fd = -1;
	cmd->append = 0;
	cmd->here_doc = 0;
	cmd->tokens_start = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	*new_cmd = cmd;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 20:04:08 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 01:46:17 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_data(t_data *data, char **environ)
{
	data->here_doc_temp_files = NULL;
	data->envp = NULL;
	data->input = NULL;
	data->exit_code = 0;
	data->tokens = NULL;
	data->env_var = NULL;
	data->cmds = NULL;
	data->num_cmds = 0;
	init_envp(environ, data, 0);
	data->input_file = NULL;
	data->output_file = NULL;
	data->here_doc = 0;
	data->append = 0;
	data->delimiter = NULL;
	data->input_fd = STDIN_FILENO;
	data->output_fd = STDOUT_FILENO;
}

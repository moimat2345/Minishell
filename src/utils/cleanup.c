/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 00:08:20 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/14 17:00:30 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	clean_token(t_data *data)
{
	t_tokens	*current;
	t_tokens	*next_token;

	current = *(data->tokens);
	if (*(data->tokens))
	{
		current = *(data->tokens);
		while (current)
		{
			next_token = current->next;
			if (current->value)
				free(current->value);
			free(current);
			current = next_token;
		}
	}
	free(data->tokens);
	data->tokens = NULL;
}

void	clean_files(t_data *data)
{
	if (data->input_file)
	{
		free(data->input_file);
		data->input_file = NULL;
		if (data->input_fd > 2)
			close(data->input_fd);
		data->input_fd = STDIN_FILENO;
	}
	if (data->output_file)
	{
		free(data->output_file);
		data->output_file = NULL;
		if (data->output_fd > 2)
			close(data->output_fd);
		data->output_fd = STDOUT_FILENO;
	}
}

void	clean_fds(t_data *data)
{
	if (data->output_fd != STDOUT_FILENO)
	{
		close(data->output_fd);
		data->output_fd = STDOUT_FILENO;
	}
}

void	delete_temp_files(char **temp_files)
{
	int	i;

	if (!temp_files)
		return ;
	i = 0;
	while (temp_files[i])
	{
		unlink(temp_files[i]);
		i++;
	}
}

void	cleanup_for_next_command(t_data *data)
{
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->cmds)
	{
		free_cmds(&data->cmds);
		data->cmds = NULL;
	}
	if (data->tokens)
		clean_token(data);
	if (data->input_file || data->output_file)
		clean_files(data);
	if (data->here_doc_temp_files)
	{
		delete_temp_files(data->here_doc_temp_files);
		free_strs(data->here_doc_temp_files);
		data->here_doc_temp_files = NULL;
	}
	if (data->input_fd != 0 || data->output_fd != 1)
		clean_fds(data);
}

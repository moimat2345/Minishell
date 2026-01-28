/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:38:28 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 17:00:19 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	heredoc_child_process(t_cmd *cmd, t_data *data, int store_fd)
{
	char	*line;

	set_signal(HEREDOC_CHILD);
	if (data->input_fd < 0)
		exit(1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(data->input_fd);
			close(store_fd);
			error_here_doc(data, cmd->delimiter);
			exit(0);
		}
		if (strcmp(line, cmd->delimiter) == 0)
		{
			close(data->input_fd);
			close(store_fd);
			free(line);
			exit(0);
		}
		write_heredoc_line(cmd, data, line);
		free(line);
	}
}

void	heredoc_parent_process(t_cmd *cmd, t_data *data,
	char *temp, int store_fd)
{
	int	status;

	set_signal(HEREDOC_PARENT);
	waitpid(-1, &status, 0);
	if (g_exit_status == 130)
	{
		close(data->input_fd);
		close(store_fd);
		unlink(temp);
		return ;
	}
	close(data->input_fd);
	data->input_fd = open(temp, O_RDONLY);
	if (data->input_fd < 0)
	{
		close(store_fd);
		return (error_minishell("Can't open heredoc file\n", 1, &data));
	}
	if (cmd->input_fd > 2)
		close(cmd->input_fd);
	cmd->input_fd = data->input_fd;
	data->input_fd = store_fd;
	unlink(temp);
	set_signal(MAIN_SIGNAL);
}

void	handle_here_doc(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	char	*temp_filename;
	int		store_fd;

	temp_filename = make_temp_filename(data);
	store_fd = dup(data->input_fd);
	if (store_fd < 0)
	{
		free(temp_filename);
		return (error_minishell("Failed to save stdin\n", 1, &data));
	}
	data->input_fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	pid = fork();
	if (pid == -1)
	{
		free(temp_filename);
		return (error_minishell("fork failed\n", 1, &data));
	}
	if (pid == 0)
		heredoc_child_process(cmd, data, store_fd);
	else
		heredoc_parent_process(cmd, data, temp_filename, store_fd);
	free(temp_filename);
}

int	process_here_docs(t_data *data)
{
	t_tokens	*token;
	t_cmd		*current_cmd;

	token = *(data->tokens);
	current_cmd = data->cmds;
	while (token && current_cmd)
	{
		if (token->type == HEREDOC)
		{
			handle_here_doc(current_cmd, data);
			if (data->exit_code == 130)
			{
				return (0);
			}
		}
		if (token->type == PIPE && current_cmd->next)
			current_cmd = current_cmd->next;
		token = token->next;
	}
	return (1);
}

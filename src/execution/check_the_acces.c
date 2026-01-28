/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_the_acces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 16:39:14 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 13:36:18 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_output_dir(char *filename, t_data **data)
{
	char	*dir_path;

	dir_path = get_directory_path(filename);
	if (dir_path && access(dir_path, W_OK) != 0)
	{
		free(dir_path);
		errno = ENOTDIR;
		file_access_error(filename, data);
		return (0);
	}
	if (dir_path)
		free(dir_path);
	return (1);
}

int	check_file_access(char *filename, int is_input, t_data **data)
{
	if (!filename || filename[0] == '\0')
	{
		errno = ENOENT;
		file_access_error(filename, data);
		return (0);
	}
	if (is_input)
	{
		if (access(filename, F_OK) != 0 || access(filename, R_OK) != 0)
			return (file_access_error(filename, data), 0);
	}
	else
	{
		if (ft_strncmp(filename, "/dev/null", 9) == 0)
			return (1);
		if (!check_output_dir(filename, data))
			return (0);
		if (access(filename, F_OK) == 0 && access(filename, W_OK) != 0)
			return (file_access_error(filename, data), 0);
	}
	return (1);
}

void	open_input_files(t_cmd *cmd, t_tokens **current, t_data *data)
{
	int			fd;

	if ((*current)->type == REDIRECT_IN
		&& (*current)->next && (*current)->next->value)
	{
		if (!check_file_access((*current)->next->value, 1, &data))
		{
			check_existence_of_fd(&cmd->input_fd);
			cmd->has_redir_error = 1;
			*current = (*current)->next->next;
			return ;
		}
		fd = open((*current)->next->value, O_RDONLY);
		check_existence_of_fd(&cmd->input_fd);
		cmd->input_fd = fd;
		*current = (*current)->next->next;
	}
}

void	open_output_files(t_cmd *cmd, t_tokens **current, t_data *data)
{
	int			fd;

	if (!check_file_access((*current)->next->value, 0, &data))
	{
		check_existence_of_fd(&cmd->output_fd);
		cmd->has_redir_error = 1;
		*current = (*current)->next->next;
		return ;
	}
	if ((*current)->type == APPEND)
		fd = open((*current)->next->value, 02101, 0644);
	else
		fd = open((*current)->next->value, 01101, 0644);
	check_existence_of_fd(&cmd->output_fd);
	cmd->output_fd = fd;
	*current = (*current)->next->next;
}

void	open_cmd_files(t_cmd *cmd, t_data *data)
{
	t_tokens	*current;

	current = cmd->tokens_start;
	if (!cmd->tokens_start)
		return ;
	while (current && current->type != PIPE && !cmd->has_redir_error)
	{
		if (current->type == REDIRECT_IN
			&& current->next && current->next->value)
			open_input_files(cmd, &current, data);
		else if ((current->type == REDIRECT_OUT || current->type == APPEND)
			&& current->next && current->next->value)
			open_output_files(cmd, &current, data);
		else
			current = current->next;
	}
	if (cmd->has_redir_error)
	{
		check_existence_of_fd(&cmd->input_fd);
		check_existence_of_fd(&cmd->output_fd);
		return ;
	}
	(void)data;
}

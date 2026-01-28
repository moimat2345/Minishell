/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 09:49:20 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/14 14:35:53 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	check_existence_of_fd(int *fd)
{
	if (*fd > 2)
		close(*fd);
	(*fd) = -1;
}

void	solo_build_in_execution_and_handling_of_fd(t_data **data, t_cmd **cmd)
{
	int		save_stdin;
	int		save_stdout;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	if ((*cmd)->input_fd > 2 || (*cmd)->here_doc)
		dup2((*cmd)->input_fd, STDIN_FILENO);
	if ((*cmd)->output_fd > 2)
		dup2((*cmd)->output_fd, STDOUT_FILENO);
	if ((*cmd)->here_doc && (*cmd)->input_fd > 2)
		close((*cmd)->input_fd);
	if ((*cmd)->args && ft_strcmp((*cmd)->args[0], "exit") == 0
		&& ((*cmd)->args[1] == NULL || ((*cmd)->args[1] != NULL
				&& (*cmd)->args[2] == NULL)))
	{
		close(save_stdin);
		close(save_stdout);
	}
	execute_build_in((*cmd), (*data));
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdin);
	close(save_stdout);
}

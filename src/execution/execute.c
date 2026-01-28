/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 15:46:11 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 14:00:24 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	duplicate_fd(int (*pipefd)[2], t_cmd *cmd, int n, int i)
{
	if (cmd->input_fd > 2 || cmd->here_doc)
		dup2(cmd->input_fd, STDIN_FILENO);
	else if (i == 0)
		dup2(STDIN_FILENO, STDIN_FILENO);
	else
		dup2(pipefd[i - 1][0], STDIN_FILENO);
	if (cmd->output_fd > 2)
		dup2(cmd->output_fd, STDOUT_FILENO);
	else if (i == n - 1)
		dup2(STDOUT_FILENO, STDOUT_FILENO);
	else
		dup2(pipefd[i][1], STDOUT_FILENO);
	if (cmd->here_doc && cmd->input_fd > 2)
		close(cmd->input_fd);
	check_existence_of_fd(&cmd->input_fd);
	check_existence_of_fd(&cmd->output_fd);
	close_all_pipes(pipefd, n - 1);
}

void	execute_child(t_cmd *cmd, t_data *data, t_envp **envp,
	int (*pipefd)[2])
{
	int	i;

	set_signal(CHILD_SIGNAL);
	if (cmd->has_redir_error)
	{
		close_all_pipes(pipefd, data->num_cmds - 1);
		exit(1);
	}
	open_cmd_files(cmd, data);
	if (cmd->has_redir_error)
	{
		close_all_pipes(pipefd, data->num_cmds - 1);
		exit(1);
	}
	i = get_cmd_index(data->cmds, cmd);
	duplicate_fd(pipefd, cmd, data->num_cmds, i);
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		exit(0);
	if (cmd->is_built_in)
		execute_build_in(cmd, data);
	else
		execute_external_function(cmd, data, envp);
	free_envp(envp);
	exit(data->exit_code);
}

void	check_if_malloc_failed_plus_create_pipe(int (*pipefd)[2], t_data *data,
	pid_t *pids)
{
	if (!pipefd)
		exit_with_error("Memory allocation failed", 1, &data);
	if (!pids)
	{
		free(pipefd);
		exit_with_error("Memory allocation failed", 1, &data);
	}
	create_pipes(pipefd, data->num_cmds);
}

int	process(t_data *data, t_cmd *cmd_original, t_envp **envp)
{
	int		(*pipefd)[2];
	pid_t	*pids;
	t_cmd	*cmd;
	int		i;

	pipefd = malloc(sizeof(int [2]) * (data->num_cmds - 1));
	pids = malloc(sizeof(pid_t) * data->num_cmds);
	check_if_malloc_failed_plus_create_pipe(pipefd, data, pids);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	i = -1;
	cmd = cmd_original;
	while (++i < data->num_cmds)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			execute_child(cmd, data, envp, pipefd);
		cmd = cmd->next;
	}
	close_all_pipes(pipefd, data->num_cmds - 1);
	wait_all_children(data->num_cmds, data, pids);
	set_signal(MAIN_SIGNAL);
	free(pipefd);
	free(pids);
	return (0);
}

void	execution(t_data *data, t_envp **envp)
{
	t_cmd	*cmd;

	cmd = data->cmds;
	if (data->num_cmds == 1 && cmd->is_built_in)
	{
		if (cmd->has_redir_error)
		{
			data->exit_code = 1;
			return ;
		}
		open_cmd_files(cmd, data);
		if (cmd->has_redir_error)
		{
			data->exit_code = 1;
			return ;
		}
		solo_build_in_execution_and_handling_of_fd(&data, &cmd);
		check_existence_of_fd(&cmd->input_fd);
		check_existence_of_fd(&cmd->output_fd);
		return ;
	}
	process(data, data->cmds, envp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 04:59:00 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 05:26:56 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_tilde(char *cmd, t_envp **envp)
{
	char	*home;
	char	*expanded;

	if (cmd[0] != '~')
		return (ft_strdup(cmd));
	if (cmd[1] != '\0' && cmd[1] != '/')
		return (ft_strdup(cmd));
	home = find_in_envp("HOME", *envp);
	if (!home)
		return (ft_strdup(cmd));
	if (cmd[1] == '\0')
		return (ft_strdup(home));
	expanded = ft_strjoin(home, cmd + 1);
	return (expanded);
}

int	handle_child_status(int status)
{
	int	exit_code;

	exit_code = 0;
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGPIPE)
			exit_code = 0;
		else
		{
			exit_code = (128 + WTERMSIG(status)) % 256;
			if (WTERMSIG(status) == SIGINT)
				ft_putstr_fd("\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", 1);
		}
	}
	else if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status) % 256;
	return (exit_code);
}

void	wait_all_children(int n, t_data *data, pid_t *pids)
{
	int		i;
	int		status;
	int		last_exit_code;
	pid_t	last_pid;

	i = 0;
	last_exit_code = 0;
	last_pid = pids[n - 1];
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (pids[i] == last_pid)
			last_exit_code = handle_child_status(status);
		i++;
	}
	data->exit_code = last_exit_code;
}

void	create_pipes(int (*pipefd)[2], int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipefd[i]) == -1)
		{
			free(pipefd);
			exit_with_error("Pipe creation failed", 1, NULL);
		}
		i++;
	}
}

void	close_all_pipes(int (*pipefd)[2], int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipefd[i][0] >= 0)
			close(pipefd[i][0]);
		if (pipefd[i][1] >= 0)
			close(pipefd[i][1]);
		i++;
	}
}

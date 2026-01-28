/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 02:42:02 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 02:42:18 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ctrl_c(int sig)
{
	g_exit_status = 130;
	ft_putstr_fd("\n", 1);
	(void)sig;
}

void	ctrl_backslash(int sig)
{
	g_exit_status = 131;
	ft_putstr_fd("Quit: (core dumped)\n", 1);
	(void)sig;
}

void	reset_prompt(int sig)
{
	g_exit_status = 130;
	ft_putstr_fd("\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	heredoc_child(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 130;
		ft_putstr_fd("\n", 1);
		exit(g_exit_status);
	}
}

void	heredoc_parent(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 130;
		ft_putstr_fd("\n", 1);
	}
}

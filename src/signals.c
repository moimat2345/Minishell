/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 22:27:32 by Nikita_Kuyd       #+#    #+#             */
/*   Updated: 2026/01/09 02:42:14 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_signal(int signal_type)
{
	if (signal_type == MAIN_SIGNAL)
	{
		signal(SIGINT, reset_prompt);
		signal(SIGQUIT, SIG_IGN);
	}
	if (signal_type == CHILD_SIGNAL)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, ctrl_backslash);
	}
	if (signal_type == HEREDOC_CHILD)
	{
		signal(SIGINT, heredoc_child);
		signal(SIGQUIT, SIG_IGN);
	}
	if (signal_type == HEREDOC_PARENT)
	{
		signal(SIGINT, heredoc_parent);
		signal(SIGQUIT, SIG_IGN);
	}
}

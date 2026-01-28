/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 02:14:41 by mateon            #+#    #+#             */
/*   Updated: 2026/01/07 07:25:26 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_commands(t_tokens **tokens)
{
	t_tokens	*current;
	int			count;

	current = *tokens;
	count = 1;
	while (current)
	{
		if (current->type == PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

int	count_args(t_tokens *token)
{
	int			count;
	t_tokens	*current;

	count = 0;
	current = token;
	while (current && current->type != PIPE)
	{
		if (current->type == COMMAND || current->type == FLAG
			|| current->type == STRING)
		{
			count++;
			current = current->next;
		}
		else if (is_redirection_token(current->type))
		{
			current = current->next;
			if (current)
				current = current->next;
		}
		else
			current = current->next;
	}
	return (count);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 20:50:59 by mateon            #+#    #+#             */
/*   Updated: 2026/01/08 17:16:36 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parsing(t_data *data)
{
	if (!data->tokens || *(data->tokens) == NULL)
		return (0);
	if (!check_tokens_syntax(data))
	{
		data->exit_code = 2;
		return (0);
	}
	cut_to_cmds(data);
	if (!data->cmds)
	{
		data->exit_code = 2;
		return (0);
	}
	if (!process_here_docs(data))
	{
		data->exit_code = 130;
		return (0);
	}
	return (1);
}

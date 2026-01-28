/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_int.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 04:49:42 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 05:22:18 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_var_name_len(char *s, int i)
{
	int		len;

	len = 0;
	while (s[i] && is_a_dollar_char(s[i]))
	{
		len++;
		i++;
	}
	return (len);
}

int	get_cmd_index(t_cmd *start, t_cmd *target)
{
	int	i;

	i = 0;
	while (start && start != target)
	{
		start = start->next;
		i++;
	}
	return (i);
}

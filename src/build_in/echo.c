/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:31:56 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 12:02:48 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_the_flag(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (1);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	build_in_echo(t_data *data, char **args)
{
	int	i;
	int	newline;
	int	args_count;

	newline = 1;
	i = 1;
	args_count = ft_split_len(args);
	while (i < args_count && check_the_flag(args[i]) == 0)
	{
		newline = 0;
		i++;
	}
	while (i < args_count)
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (i < args_count - 1)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	data->exit_code = 0;
}

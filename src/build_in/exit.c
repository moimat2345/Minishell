/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:31:52 by mateon            #+#    #+#             */
/*   Updated: 2026/01/08 22:02:41 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parser_exit_argument(char **args, t_data **data, int args_c)
{
	int	i;

	if (args_c == 1)
		return (0);
	i = 0;
	if (args[1][0] == '+' || args[1][0] == '-')
		i++;
	if (args[1][i] == '\0')
	{
		error_minishell("exit: numeric argument required\n", 2, data);
		return (-1);
	}
	while (args[1][i])
	{
		if (ft_isdigit(args[1][i]) == 0)
		{
			error_minishell("exit: numeric argument required\n", 2, data);
			return (-1);
		}
		i++;
	}
	return (0);
}

void	exit_exit(char *message, int exit_code, t_data **data)
{
	ft_putstr_fd(message, 1);
	if (data && *data)
	{
		cleanup_for_next_command(*data);
		if ((*data)->env_var)
			free_envp(&(*data)->env_var);
		if ((*data)->envp)
			free_strs((*data)->envp);
		rl_clear_history();
		free(*data);
	}
	exit(exit_code);
}

void	build_in_exit(t_data *data, char **args)
{
	long long	exit_code;
	long long	arg;
	int			args_count;

	args_count = ft_split_len(args);
	if (parser_exit_argument(args, &data, args_count) == -1)
		exit_exit("exit\n", 2, &data);
	if (args_count > 2)
	{
		error_minishell("exit: too many arguments\n", 1, &data);
		return ;
	}
	if (args_count == 2)
	{
		arg = ft_atoll(args[1]);
		exit_code = ((unsigned char)arg);
		exit_exit("exit\n", exit_code, &data);
	}
	else
		exit_exit("exit\n", data->exit_code, &data);
}

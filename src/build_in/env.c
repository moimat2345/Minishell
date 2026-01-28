/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:31:54 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 12:04:52 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	build_in_env(t_data *data, char **args)
{
	t_envp	*env;

	env = data->env_var;
	if (args[1] != NULL)
	{
		error_minishell("env: too many arguments\n", 1, &data);
		return ;
	}
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		env = env->next;
	}
	data->exit_code = 0;
}

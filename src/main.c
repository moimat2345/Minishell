/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateon <mateon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 20:37:07 by mateon            #+#    #+#             */
/*   Updated: 2026/01/28 17:47:37 by mateon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_exit_status;

void	main_process(t_data *data, int exit_code)
{
	if (!data->input)
	{
		exit_code = data->exit_code;
		ft_putstr_fd("exit\n", 1);
		free_envp(&(data->env_var));
		cleanup_for_next_command(data);
		if (data->envp)
			free(data->envp);
		rl_clear_history();
		free(data);
		exit(exit_code);
	}
	if (data->input[0] != '\0')
	{
		add_history(data->input);
	}
	lexer_main(data);
	if (parsing(data))
	{
		if (g_exit_status == 130)
			data->exit_code = 130;
		execution(data, &(data->env_var));
	}
	cleanup_for_next_command(data);
	g_exit_status = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	(void)argv;
	if (argc > 1)
		exit_with_error("Minishell does not take arguments", 0, NULL);
	if (envp == NULL)
		exit_with_error("Environment variables are required", 1, NULL);
	data = malloc(sizeof(t_data));
	if (!data)
		exit_with_error("Memory allocation failed", 1, NULL);
	header();
	init_data(data, envp);
	while (1)
	{
		set_signal(MAIN_SIGNAL);
		data->input = readline("minishell$ ");
		main_process(data, 0);
	}
	return (0);
}

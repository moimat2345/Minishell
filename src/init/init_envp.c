/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 14:19:59 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 13:56:07 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_first_or_new_envp(t_data *data, t_envp *new_var)
{
	t_envp	*last_var;

	if (data->env_var == NULL)
	{
		new_var->prev = NULL;
		data->env_var = new_var;
	}
	else
	{
		last_var = data->env_var;
		while (last_var->next)
			last_var = last_var->next;
		last_var->next = new_var;
		new_var->prev = last_var;
	}
}

void	increment_shlvl(t_data *data)
{
	t_envp	*current;
	int		shlvl;
	t_envp	*new_var;
	char	*old_shlvl;

	current = data->env_var;
	while (current)
	{
		if (ft_strcmp(current->name, "SHLVL") == 0)
		{
			shlvl = ft_atoi(current->value) + 1;
			old_shlvl = current->value;
			current->value = ft_itoa(shlvl);
			free(old_shlvl);
			return ;
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_envp));
	if (!new_var)
		exit_with_error("Memory allocation failed", 1, &data);
	new_var->name = ft_strdup("SHLVL");
	new_var->value = ft_strdup("1");
	new_var->next = NULL;
	init_first_or_new_envp(data, new_var);
}

void	init_envp(char **envp, t_data *data, int i)
{
	t_envp		*new_var;
	char		*equal_sign;

	data->env_var = NULL;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			new_var = malloc(sizeof(t_envp));
			if (!new_var)
				exit_with_error("Memory allocation failed", 1, &data);
			new_var->name = ft_strndup(envp[i], equal_sign - envp[i]);
			equal_sign++;
			if (equal_sign)
				new_var->value = ft_strdup(equal_sign);
			else
				new_var->value = NULL;
			new_var->next = NULL;
			init_first_or_new_envp(data, new_var);
		}
		i++;
	}
	increment_shlvl(data);
}

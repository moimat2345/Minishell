/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:31:50 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 06:30:59 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_envp	*find_node_in_envp(t_envp *env, char *name)
{
	t_envp	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	update_envp_value(t_envp *node, char *new_value)
{
	if (node->value)
		free(node->value);
	if (new_value != NULL)
		node->value = ft_strdup(new_value);
	else
		node->value = NULL;
}

void	add_in_envp_struct(t_data *data, char *name, char *value)
{
	t_envp	*new_env;
	t_envp	*last;

	new_env = (t_envp *)malloc(sizeof(t_envp));
	if (!new_env)
		return ;
	new_env->name = ft_strdup(name);
	if (value)
		new_env->value = ft_strdup(value);
	else
		new_env->value = NULL;
	new_env->next = NULL;
	new_env->prev = NULL;
	if (data->env_var == NULL)
	{
		data->env_var = new_env;
	}
	else
	{
		last = data->env_var;
		while (last->next)
			last = last->next;
		last->next = new_env;
		new_env->prev = last;
	}
}

int	process_export_arg(t_data *data, char *arg)
{
	char	*name;
	char	*value;
	int		has_equal;

	name = NULL;
	value = NULL;
	has_equal = 0;
	if (parser_export_argument(arg, &name, &value, &has_equal))
	{
		if (name)
		{
			check_which_add(data, name, value, has_equal);
			free(name);
			free(value);
		}
		return (0);
	}
	return (1);
}

void	build_in_export(t_data *data, char **args)
{
	int		i;
	int		has_error;

	has_error = 0;
	if (args[1] == NULL || args[1][0] == '\0')
	{
		print_sorted_envp(data);
		data->exit_code = 0;
		return ;
	}
	i = 1;
	while (args[i] && args[i][0] != '\0')
	{
		if (process_export_arg(data, args[i]))
			has_error = 1;
		i++;
	}
	if (has_error)
		data->exit_code = 1;
	else
		data->exit_code = 0;
}

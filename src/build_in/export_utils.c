/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 06:10:22 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 06:10:49 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_env(t_data *data)
{
	int		count;
	t_envp	*current;

	count = 0;
	current = data->env_var;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	sort_envp_array(t_envp **env_array, int size)
{
	int		i;
	int		j;
	t_envp	*temp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(env_array[j]->name, env_array[j + 1]->name) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_export(t_envp *env_ar)
{
	if (env_ar->value != NULL)
		printf("declare -x %s=\"%s\"\n", env_ar->name, env_ar->value);
	else
		printf("declare -x %s\n", env_ar->name);
}

void	print_sorted_envp(t_data *data)
{
	int		size;
	int		i;
	t_envp	**env_ar;
	t_envp	*current;

	size = count_env(data);
	env_ar = (t_envp **)malloc(sizeof(t_envp *) * size);
	if (!env_ar)
		return ;
	current = data->env_var;
	i = 0;
	while (current)
	{
		env_ar[i++] = current;
		current = current->next;
	}
	sort_envp_array(env_ar, size);
	i = 0;
	while (i < size)
	{
		print_export(env_ar[i]);
		i++;
	}
	free(env_ar);
}

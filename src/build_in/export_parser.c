/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 06:07:25 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 06:08:35 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || name[0] == '\0' || (ft_isalpha(name[0]) == 0
			&& name[0] != '_'))
		return (0);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (ft_isalnum(name[i]) == 0 && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	parse_with_equal(char *arg, char **name, char **value, char *equal_sign)
{
	size_t	name_len;

	name_len = equal_sign - arg;
	*name = (char *)malloc(name_len + 1);
	if (!*name)
		return (0);
	ft_strlcpy(*name, arg, name_len + 1);
	if (!is_valid_identifier(*name))
	{
		print_export_error(arg);
		free(*name);
		return (0);
	}
	*value = ft_strdup(equal_sign + 1);
	return (1);
}

int	parser_export_argument(char *arg, char **name, char **value, int *has_equal)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign == NULL)
	{
		if (!is_valid_identifier(arg))
		{
			print_export_error(arg);
			return (0);
		}
		*name = ft_strdup(arg);
		*value = NULL;
		*has_equal = 0;
		return (1);
	}
	else
	{
		if (!parse_with_equal(arg, name, value, equal_sign))
			return (0);
		*has_equal = 1;
	}
	return (1);
}

void	check_which_add(t_data *data, char *name, char *value, int has_equal)
{
	t_envp	*node;

	node = find_node_in_envp(data->env_var, name);
	if (node)
	{
		if (has_equal)
			update_envp_value(node, value);
	}
	else
	{
		add_in_envp_struct(data, name, value);
	}
}

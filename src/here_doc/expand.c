/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 06:12:52 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 07:45:08 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_heredoc_var(char *line, char *dest, int *ij, t_data *data)
{
	int		var_len;
	char	*var_name;
	char	*var_value;
	int		i_val;

	i_val = ij[0];
	i_val++;
	var_len = get_var_name_len(line, i_val);
	if (var_len == 0)
	{
		ij[0] = i_val;
		return ;
	}
	var_name = get_var_name(line, i_val, var_len);
	if (!var_name)
		return ;
	var_value = find_in_envp(var_name, data->env_var);
	if (var_value)
		copy_var_to_dest(dest, var_value, &ij[1]);
	free(var_name);
	ij[0] = i_val + var_len;
}

int	calc_expanded_len(char *line, t_data *data)
{
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '?')
		{
			len += ft_strlen(ft_itoa(data->exit_code));
			i += 2;
		}
		else if (line[i] == '$' && is_a_dollar_char(line[i + 1]))
			len += add_var_len_to_total(line, &i, data->env_var);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

void	expand_exit_code_to_dest(char *exp, int *j, t_data *data, int *i)
{
	char	*exit_code;

	exit_code = ft_itoa(data->exit_code);
	ft_strcpy(exp + *j, exit_code);
	*j += ft_strlen(exit_code);
	free(exit_code);
	*i += 2;
}

char	*expand_heredoc_line(char *line, t_data *data)
{
	char	*expanded;
	int		ij[2];

	expanded = malloc(sizeof(char) * (calc_expanded_len(line, data) + 1));
	if (!expanded)
		return (ft_strdup(line));
	ij[0] = 0;
	ij[1] = 0;
	while (line[ij[0]])
	{
		if (line[ij[0]] == '$' && line[ij[0] + 1] == '?')
			expand_exit_code_to_dest(expanded, &ij[1], data, &ij[0]);
		else if (line[ij[0]] == '$' && is_a_dollar_char(line[ij[0] + 1]))
			expand_heredoc_var(line, expanded, ij, data);
		else
			expanded[ij[1]++] = line[ij[0]++];
	}
	expanded[ij[1]] = '\0';
	return (expanded);
}

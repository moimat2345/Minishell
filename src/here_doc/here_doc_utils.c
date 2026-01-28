/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 06:15:18 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 07:45:22 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	copy_var_to_dest(char *dest, char *var_value, int *j)
{
	int	k;

	k = 0;
	while (var_value[k])
		dest[(*j)++] = var_value[k++];
}

int	add_var_len_to_total(char *line, int *i, t_envp *env)
{
	int		var_len;
	char	*var_name;
	char	*var_value;
	int		len;

	(*i)++;
	var_len = get_var_name_len(line, *i);
	var_name = get_var_name(line, *i, var_len);
	var_value = find_in_envp(var_name, env);
	len = 0;
	if (var_value)
		len = ft_strlen(var_value);
	free(var_name);
	*i += var_len;
	return (len);
}

char	*build_temp_name(int num)
{
	char	*file_number;
	char	*filename;

	file_number = ft_itoa(num);
	filename = ft_strjoin("/tmp/here_doc_", file_number);
	free(file_number);
	return (filename);
}

char	*make_temp_filename(t_data *data)
{
	char	*filename;
	int		i;
	int		base;

	base = ft_split_len(data->here_doc_temp_files);
	filename = build_temp_name(base);
	i = 1;
	while (access(filename, F_OK) == 0)
	{
		free(filename);
		filename = build_temp_name(base + i);
		i++;
	}
	return (filename);
}

void	write_heredoc_line(t_cmd *cmd, t_data *data, char *line)
{
	char	*expanded_line;

	if (cmd->heredoc_expand)
	{
		expanded_line = expand_heredoc_line(line, data);
		write(data->input_fd, expanded_line, strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(data->input_fd, line, strlen(line));
	write(data->input_fd, "\n", 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:50:40 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 06:06:22 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	error_syntax(t_data *data, char *value)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (value)
		ft_putstr_fd(value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	data->exit_code = 2;
	return (0);
}

void	error_here_doc(t_data *data, char *message)
{
	ft_putstr_fd("warning: here-document delimited \
by end-of-file (wanted '", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("')\n", 2);
	data->exit_code = 130;
}

void	error_minishell(char *message, int exit_code, t_data **data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(message, 2);
	if (data && *data)
		(*data)->exit_code = exit_code;
}

void	file_access_error(char *filename, t_data **data)
{
	char	*err_msg;

	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	err_msg = strerror(errno);
	ft_putstr_fd(err_msg, 2);
	ft_putstr_fd("\n", 2);
	if (data && *data)
		(*data)->exit_code = 1;
}

void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

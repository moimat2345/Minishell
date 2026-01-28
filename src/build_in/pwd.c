/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:31:46 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 12:05:08 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	build_in_pwd(t_data *data)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (error_minishell
			("pwd: error retrieving current directory\n", 1, &data));
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	data->exit_code = 0;
}

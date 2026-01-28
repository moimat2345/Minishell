/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:31:59 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 04:56:23 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_pwd_envvar(t_data *data)
{
	t_envp	*current;
	char	cwd[4096];

	current = data->env_var;
	while (current)
	{
		if (ft_strcmp(current->name, "PWD") == 0)
		{
			if (getcwd(cwd, sizeof(cwd)) != NULL)
			{
				free(current->value);
				current->value = ft_strdup(cwd);
			}
			return ;
		}
		current = current->next;
	}
}

void	update_oldpwd_envvar(t_data *data, char *oldpwd)
{
	t_envp	*current;

	current = data->env_var;
	while (current)
	{
		if (ft_strcmp(current->name, "OLDPWD") == 0)
		{
			free(current->value);
			if (oldpwd)
				current->value = ft_strdup(oldpwd);
			else
				current->value = ft_strdup("");
			return ;
		}
		current = current->next;
	}
}

void	build_in_cd(t_data *data, char **args)
{
	char	*path;
	char	*pwd;

	if (ft_split_len(args) > 2)
		return (error_minishell("cd: too many arguments\n", 1, &data));
	path = get_cd_path(data, args);
	if (path == NULL)
		return ;
	pwd = find_in_envp("PWD", data->env_var);
	if (pwd)
		pwd = ft_strdup(pwd);
	if (chdir(path) == -1)
	{
		free(path);
		if (pwd)
			free(pwd);
		return (error_minishell("cd: No such file or directory\n", 1, &data));
	}
	free(path);
	update_pwd_envvar(data);
	update_oldpwd_envvar(data, pwd);
	if (pwd)
		free(pwd);
	data->exit_code = 0;
}

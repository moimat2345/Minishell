/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_char.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 04:48:06 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 10:31:34 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_directory_path(char *filename)
{
	char	*last_slash;
	char	*dir_path;

	last_slash = strrchr(filename, '/');
	if (last_slash)
	{
		dir_path = malloc(last_slash - filename + 2);
		if (!dir_path)
			return (NULL);
		ft_strlcpy(dir_path, filename, last_slash - filename + 2);
		return (dir_path);
	}
	return (NULL);
}

char	*handle_tilde(char **args, char **path, t_data *data)
{
	char	*home_path;
	char	*final_path;

	home_path = find_in_envp("HOME", data->env_var);
	if (home_path == NULL)
	{
		error_minishell("cd: HOME not set\n", 1, &data);
		return (NULL);
	}
	if (args[1][1] == '\0')
	{
		*path = ft_strdup(home_path);
		return (*path);
	}
	final_path = ft_strjoin(home_path, args[1] + 1);
	*path = final_path;
	return (*path);
}

char	*get_cd_path(t_data *data, char **args)
{
	char	*path;
	char	*temp;

	if (ft_split_len(args) == 1)
	{
		temp = find_in_envp("HOME", data->env_var);
		if (temp == NULL)
			return (error_minishell("cd: HOME not set\n", 1, &data), NULL);
		path = ft_strdup(temp);
	}
	else if (strcmp(args[1], "-") == 0)
	{
		temp = find_in_envp("OLDPWD", data->env_var);
		if (temp == NULL)
			return (error_minishell("cd: OLDPWD not set\n", 1, &data), NULL);
		printf("%s\n", temp);
		path = ft_strdup(temp);
	}
	else if (strncmp(args[1], "~", 1) == 0)
		path = handle_tilde(args, &path, data);
	else
		path = ft_strdup(args[1]);
	return (path);
}

char	*get_var_name(char *s, int i, int var_len)
{
	char	*var_name;
	int		k;

	var_name = malloc(sizeof(char) * (var_len + 1));
	if (!var_name)
		return (NULL);
	k = 0;
	while (k < var_len)
	{
		var_name[k] = s[i];
		k++;
		i++;
	}
	var_name[k] = '\0';
	return (var_name);
}

char	**get_path(t_envp **env)
{
	t_envp	*envp;

	envp = *env;
	while (envp)
	{
		if (ft_strncmp(envp->name, "PATH", 5) == 0)
			return (ft_split(envp->value, ':'));
		envp = envp->next;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:49:40 by nkuydin           #+#    #+#             */
/*   Updated: 2026/01/09 10:33:06 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin_with_slash(char *s1, char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = (char *)malloc(len1 + len2 + 2);
	if (!joined)
		return (NULL);
	ft_strlcpy(joined, s1, len1 + 1);
	joined[len1] = '/';
	ft_strlcpy(joined + len1 + 1, s2, len2 + 1);
	return (joined);
}

char	*check_absolute_path(char *expanded_cmd, t_data *data)
{
	struct stat	st;

	if (stat(expanded_cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		free(expanded_cmd);
		error_minishell("Is a directory\n", 126, &data);
		return (NULL);
	}
	if (access(expanded_cmd, F_OK) == 0)
	{
		if (access(expanded_cmd, X_OK) == 0)
			return (expanded_cmd);
		free(expanded_cmd);
		error_minishell("Permission denied\n", 126, &data);
		return (NULL);
	}
	free(expanded_cmd);
	error_minishell("No such file or directory\n", 127, &data);
	return (NULL);
}

char	*search_in_paths(char **paths, char *cmd, t_data *data)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin_with_slash(paths[i], cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			if (access(cmd_path, X_OK) == 0)
			{
				free_split(paths);
				return (cmd_path);
			}
			free(cmd_path);
			free_split(paths);
			error_minishell("Permission denied\n", 126, &data);
			return (NULL);
		}
		free(cmd_path);
		i++;
	}
	free_split(paths);
	error_minishell("command not found\n", 127, &data);
	return (NULL);
}

char	*find_command_path(char *cmd, t_envp **envp, t_data *data)
{
	char		**paths;
	char		*expanded_cmd;

	if (ft_strcmp(cmd, "..") == 0)
	{
		error_minishell("command not found\n", 127, &data);
		return (NULL);
	}
	if (ft_strcmp(cmd, ".") == 0)
	{
		error_minishell(".: filename argument required\n\
.: usage: . filename [arguments]\n", 2, &data);
		return (NULL);
	}
	expanded_cmd = expand_tilde(cmd, envp);
	if (!expanded_cmd)
		expanded_cmd = ft_strdup(cmd);
	if (ft_strchr(expanded_cmd, '/'))
		return (check_absolute_path(expanded_cmd, data));
	free(expanded_cmd);
	paths = get_path(envp);
	if (!paths)
		return (error_unset_path(cmd, &data), NULL);
	return (search_in_paths(paths, cmd, data));
}

char	*find_cmd_path(char **paths, char *cmd)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin_with_slash(paths[i], cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			if (access(cmd_path, X_OK) == 0)
				return (cmd_path);
			free(cmd_path);
			return (NULL);
		}
		free(cmd_path);
		i++;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 17:33:46 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 07:53:28 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_command_execve(char *cmd, t_data **data)
{
	if (errno == EACCES)
	{
		error_minishell("", 126, data);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else
	{
		error_minishell("", 1, data);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
}

int	count_envp(t_envp *envp)
{
	int	i;

	i = 0;
	while (envp)
	{
		i++;
		envp = envp->next;
	}
	return (i);
}

char	*create_env_string(t_envp *current)
{
	char	*temp;
	char	*result;

	if (current->value)
		temp = ft_strjoin(current->name, "=");
	else
		return (ft_strdup(current->name));
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, current->value);
	free(temp);
	return (result);
}

char	**convert_envp_to_strs(t_envp *envp)
{
	char	**envp_strs;
	int		i;

	envp_strs = malloc(sizeof(char *) * (count_envp(envp) + 1));
	if (!envp_strs)
		return (NULL);
	i = 0;
	while (envp)
	{
		envp_strs[i] = create_env_string(envp);
		if (!envp_strs[i])
		{
			while (i > 0)
				free(envp_strs[--i]);
			free(envp_strs);
			return (NULL);
		}
		envp = envp->next;
		i++;
	}
	envp_strs[i] = NULL;
	return (envp_strs);
}

void	execute_external_function(t_cmd *cmd, t_data *data, t_envp **envp)
{
	char	*cmd_path;
	char	**envp_strs;

	if (cmd->args && cmd->args[0] != NULL && cmd->args[0][0] != '\0')
		cmd_path = find_command_path((cmd->args[0]), &(data->env_var), data);
	else
		cmd_path = NULL;
	if (cmd_path == NULL)
		exit(data->exit_code);
	envp_strs = convert_envp_to_strs(*envp);
	if (envp_strs == NULL)
	{
		free(cmd_path);
		error_minishell("Memory allocation failed\n", 1, &data);
		exit(1);
	}
	if (execve(cmd_path, cmd->args, envp_strs) == -1)
	{
		free(cmd_path);
		error_command_execve(cmd->args[0], &data);
		exit(data->exit_code);
	}
	free_split(envp_strs);
	free(cmd_path);
	exit(0);
}

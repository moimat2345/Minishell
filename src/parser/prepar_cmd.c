/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepar_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 02:15:52 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 06:34:13 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	create_cmds_list(t_cmd **cmds, int total_cmds, t_data **data)
{
	t_cmd	*new_cmd;
	t_cmd	*temp;
	int		i;

	*cmds = NULL;
	i = 0;
	while (i < total_cmds)
	{
		init_new_cmd(&new_cmd, data);
		if (!new_cmd)
			exit_with_error("Memory allocation failed", 1, data);
		if (*cmds == NULL)
			*cmds = new_cmd;
		else
		{
			temp = *cmds;
			while (temp->next)
				temp = temp->next;
			temp->next = new_cmd;
			new_cmd->prev = temp;
		}
		i++;
	}
}

t_tokens	*next_token(t_tokens *token)
{
	if (is_redirection_token(token->type))
	{
		token = token->next;
		if (token)
			token = token->next;
	}
	else
		token = token->next;
	return (token);
}

char	**fill_the_args(t_tokens *token, int args_count)
{
	char	**args;
	int		i;

	args = ft_calloc(args_count + 1, sizeof(char *));
	if (!args)
		exit_with_error("Memory allocation failed", 1, &(token->data));
	i = 0;
	while (token && token->type != PIPE && i < args_count)
	{
		if (token->type == COMMAND || token->type == FLAG
			|| token->type == STRING)
		{
			args[i] = ft_strdup(token->value);
			if (!args[i])
				handle_arg_error(args, token, i);
			i++;
			token = token->next;
		}
		else
			token = next_token(token);
	}
	args[i] = NULL;
	return (args);
}

char	**extract_args(t_tokens **token)
{
	int			args_count;
	char		**args;

	args_count = count_args(*token);
	if (args_count == 0)
		return (NULL);
	args = fill_the_args(*token, args_count);
	while (*token && (*token)->type != PIPE)
		*token = (*token)->next;
	return (args);
}

void	cut_to_cmds(t_data *data)
{
	t_tokens	*current;
	t_cmd		*cmds;
	int			total_cmds;

	if (!data->tokens || *(data->tokens) == NULL)
		return ;
	total_cmds = count_commands(data->tokens);
	data->num_cmds = total_cmds;
	create_cmds_list(&(data->cmds), total_cmds, &data);
	current = *(data->tokens);
	cmds = data->cmds;
	while (current && cmds)
	{
		cmds->tokens_start = current;
		extract_the_redirection(&current, &data, cmds);
		cmds->args = extract_args(&current);
		if (current && current->type == PIPE)
			current = current->next;
		cmds = cmds->next;
	}
	found_built_in_commands(data);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 02:11:26 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 06:34:13 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redirect_in(t_cmd *cmd, t_tokens *token)
{
	char	*delimiter;
	int		has_quotes;

	if (token->type == REDIRECT_IN)
	{
		if (cmd->input_file)
			free(cmd->input_file);
		cmd->input_file = ft_strdup(token->next->value);
		cmd->here_doc = 0;
		cmd->input_fd = -1;
	}
	else if (token->type == HEREDOC)
	{
		if (cmd->delimiter)
			free(cmd->delimiter);
		delimiter = token->next->value;
		has_quotes = 0;
		if ((delimiter[0] == '"' || delimiter[0] == '\'')
			&& ft_strlen(delimiter) > 1)
			has_quotes = 1;
		cmd->delimiter = ft_strdup(delimiter);
		cmd->here_doc = 1;
		cmd->heredoc_expand = !has_quotes;
		cmd->input_fd = -1;
	}
}

void	handle_redirect_out(t_cmd *cmd, t_tokens *token)
{
	if (token->type == REDIRECT_OUT)
	{
		if (cmd->output_file)
			free(cmd->output_file);
		cmd->output_file = ft_strdup(token->next->value);
		cmd->append = 0;
		cmd->output_fd = -1;
	}
	else if (token->type == APPEND)
	{
		if (cmd->output_file)
			free(cmd->output_file);
		cmd->output_file = ft_strdup(token->next->value);
		cmd->append = 1;
		cmd->output_fd = -1;
	}
}

void	extract_the_redirection(t_tokens **token, t_data **data_in, t_cmd *cmd)
{
	t_tokens	*current;

	(void)data_in;
	current = *token;
	while (current && current->type != PIPE)
	{
		if (current->type == REDIRECT_IN || current->type == HEREDOC)
		{
			handle_redirect_in(cmd, current);
			current = current->next;
			if (current)
				current = current->next;
		}
		else if (current->type == REDIRECT_OUT || current->type == APPEND)
		{
			handle_redirect_out(cmd, current);
			current = current->next;
			if (current)
				current = current->next;
		}
		else
			current = current->next;
	}
}

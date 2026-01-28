/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:23:09 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 12:11:41 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_pipe_syntax(t_tokens *token)
{
	if (token->prev == NULL || token->next == NULL)
		return (0);
	if (token->prev->type == PIPE || is_redirection_token(token->prev->type))
		return (0);
	if (token->next->type == PIPE)
		return (0);
	return (1);
}

int	check_redirection_syntax(t_tokens *token)
{
	if (token->next == NULL)
		return (0);
	if (token->next->type == PIPE)
		return (0);
	if (is_redirection_token(token->next->type))
		return (0);
	return (1);
}

int	is_redirection_token(t_tok_type type)
{
	if (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == APPEND || type == HEREDOC)
		return (1);
	return (0);
}

int	check_type_syntax(t_tokens *token, t_data *data)
{
	if (token->type == PIPE && check_pipe_syntax(token) == 0)
		return (error_syntax(data, token->value));
	else if (is_redirection_token(token->type)
		&& check_redirection_syntax(token) == 0)
		return (error_syntax(data, token->value));
	return (1);
}

int	check_tokens_syntax(t_data *data)
{
	t_tokens	*current;

	current = *(data->tokens);
	while (current && current != NULL)
	{
		if (check_type_syntax(current, data) == 0)
			return (0);
		current = current->next;
	}
	return (1);
}

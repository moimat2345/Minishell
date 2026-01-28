/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:04:15 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 01:07:51 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_quotes_in_token(t_tokens *token, int j)
{
	char	*new_value;
	int		i;
	int		len;

	new_value = malloc(sizeof(char) * (ft_strlen(token->value) + 1));
	if (!new_value)
		return ;
	i = 1;
	len = ft_strlen(token->value);
	if ((token->value[0] == '\'' && token->value[len - 1] == '\'')
		|| (token->value[0] == '\"' && token->value[len - 1] == '\"'))
	{
		while (i < len - 1)
			new_value[j++] = token->value[i++];
		new_value[j] = '\0';
	}
	else
		ft_strlcpy(new_value, token->value, len + 1);
	if (token->value[0] == '\'' && token->value[len - 1] == '\'')
		token->simple_quote = true;
	else
		token->simple_quote = false;
	free(token->value);
	token->value = new_value;
}

void	handle_quotes(t_tokens *tokens)
{
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		handle_quotes_in_token(current, 0);
		current = current->next;
	}
}

void	unlink_token(t_tokens **tokens, t_tokens *node, t_tokens *prev)
{
	if (prev)
	{
		prev->next = node->next;
		if (node->next)
			node->next->prev = prev;
	}
	else
	{
		*tokens = node->next;
		if (node->next)
			node->next->prev = NULL;
	}
}

void	remove_empty_tokens(t_tokens **tokens)
{
	t_tokens	*current;
	t_tokens	*prev;
	t_tokens	*to_delete;

	current = *tokens;
	prev = NULL;
	while (current)
	{
		if (ft_strlen(current->value) == 0
			&& !(prev && prev->type == HEREDOC)
			&& !current->was_quoted)
		{
			to_delete = current;
			unlink_token(tokens, current, prev);
			current = current->next;
			free(to_delete->value);
			free(to_delete);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

t_tokens	*token_parser(t_tokens *tokens, t_data *data)
{
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		copy_token_value(current, data);
		current = current->next;
	}
	remove_empty_tokens(&tokens);
	return (tokens);
}

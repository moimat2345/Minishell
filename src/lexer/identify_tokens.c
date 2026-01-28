/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 14:03:05 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 07:10:35 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_tokens	*cut_lexeur(char *s)
{
	int			i;
	int			start;
	t_tokens	*the_token;
	bool		quote;

	i = 0;
	the_token = NULL;
	quote = false;
	while (is("space", s[i]))
		i++;
	if (s[i] == '\0')
		return (NULL);
	while (s[i])
	{
		while (is("space", s[i]))
			i++;
		if (s[i] == '\0')
			break ;
		if (is("special", s[i]))
			handle_specials(s, &i, &start);
		else
			handle_token(s, &i, &start);
		add_token(&the_token, ft_strndup(s + start, i - start), quote);
	}
	return (the_token);
}

int	is_redirector(t_tok_type type)
{
	if (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == APPEND || type == HEREDOC)
		return (1);
	return (0);
}

t_tok_type	get_opperator_type(char *value)
{
	if (ft_strncmp(value, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(value, ">>", 2) == 0)
		return (APPEND);
	else if (ft_strncmp(value, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(value, ">", 1) == 0)
		return (REDIRECT_OUT);
	else if (ft_strncmp(value, "<", 1) == 0)
		return (REDIRECT_IN);
	return (NONE);
}

int	in_or_out(t_tokens *current)
{
	if (current->prev->type == REDIRECT_IN || current->prev->type == HEREDOC)
		return (STDIN);
	else if (current->prev->type == REDIRECT_OUT
		|| current->prev->type == APPEND)
		return (STDOUT);
	return (STRING);
}

void	identify_tokens(t_tokens *token)
{
	t_tokens	*current;

	current = token;
	while (current)
	{
		current->type = get_opperator_type(current->value);
		if (current->type == NONE)
		{
			if (current->prev && is_redirector(current->prev->type))
				current->type = in_or_out(current);
			else if (!current->prev || current->prev->type == PIPE)
				current->type = COMMAND;
			else
				current->type = STRING;
		}
		current = current->next;
	}
}

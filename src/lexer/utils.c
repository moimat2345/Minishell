/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 21:44:51 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 03:33:11 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_a_dollar_char(char c)
{
	if (ft_isalpha(c) || ft_isdigit(c) || c == '_')
		return (1);
	return (0);
}

int	is(char *what, char c)
{
	if (ft_strncmp(what, "space", 5) == 0)
	{
		if (c == ' ' || c == '\t' || c == '\n')
			return (1);
	}
	if (ft_strncmp(what, "quote", 5) == 0)
	{
		if (c == '\'' || c == '\"')
			return (1);
	}
	if (ft_strncmp(what, "special", 7) == 0)
	{
		if (c == '|' || c == '<' || c == '>')
			return (1);
	}
	if (ft_strncmp(what, "pipe", 4) == 0)
	{
		if (c == '|')
			return (1);
	}
	return (0);
}

void	handle_specials(char *s, int *i, int *start)
{
	*start = *i;
	if (s[*i] == '>' && s[*i + 1] == '>')
		*i += 2;
	else if (s[*i] == '<' && s[*i + 1] == '<')
		*i += 2;
	else
		(*i)++;
}

void	handle_token(char *s, int *i, int *start)
{
	char	quote_char;

	*start = *i;
	while (s[*i] && !is("space", s[*i]) && !is("special", s[*i]))
	{
		if (is("quote", s[*i]))
		{
			quote_char = s[*i];
			(*i)++;
			while (s[*i] && s[*i] != quote_char)
				(*i)++;
			if (s[*i] == quote_char)
				(*i)++;
		}
		else
			(*i)++;
	}
}

char	*find_in_envp(char *the_dollar, t_envp *env_var)
{
	t_envp	*current;

	current = env_var;
	while (current && current != NULL)
	{
		if (current->name && ft_strcmp(current->name, the_dollar) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

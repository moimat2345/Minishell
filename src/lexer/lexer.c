/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 06:05:38 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 09:28:09 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_newtoken(t_tokens *new_token, char *value, bool sp_quotes)
{
	new_token->value = value;
	new_token->simple_quote = sp_quotes;
	new_token->was_quoted = false;
	new_token->next = NULL;
	new_token->env_var = NULL;
	new_token->data = NULL;
}

void	add_token(t_tokens **rendu, char *value, bool sp_quotes)
{
	t_tokens	*new_token;
	t_tokens	*temp;

	new_token = malloc(sizeof(t_tokens));
	if (!new_token)
	{
		free(value);
		return ;
	}
	set_newtoken(new_token, value, sp_quotes);
	if (*rendu == NULL)
	{
		new_token->position = 0;
		new_token->prev = NULL;
		new_token->type = NONE;
		*rendu = new_token;
		return ;
	}
	temp = *rendu;
	while (temp->next)
		temp = temp->next;
	new_token->position = temp->position + 1;
	new_token->prev = temp;
	new_token->type = NONE;
	temp->next = new_token;
}

void	lexer_main(t_data *data)
{
	t_tokens	*token_list;

	token_list = cut_lexeur(data->input);
	identify_tokens(token_list);
	token_list = token_parser(token_list, data);
	if (!data->tokens)
		data->tokens = malloc(sizeof(t_tokens *));
	if (!data->tokens)
		return ;
	*(data->tokens) = token_list;
}

void	quotes_handler(t_tokens *token, char *new_value, int *i, int *j)
{
	int		quoted_len;

	if (token->value[(*i)] == '\'')
	{
		quoted_len = copy_single_quote(token->value, new_value, i, j);
		if (quoted_len == 0)
			token->was_quoted = true;
	}
	else if (token->value[(*i)] == '\"')
	{
		quoted_len = copy_double_quote(token, new_value, i, j);
		if (quoted_len == 0)
			token->was_quoted = true;
	}
	else if (token->value[(*i)] == '$' && (token->value[(*i) + 1] != ' '
			&& token->value[(*i) + 1] != '\0'))
		check_if_special_var(token, new_value, i, j);
	else
		new_value[(*j)++] = token->value[(*i)++];
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 12:13:31 by mateon            #+#    #+#             */
/*   Updated: 2026/01/09 09:28:00 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	copy_exp_var(t_tokens *token, char *dest, int *i, int *j)
{
	int		var_len;
	char	*var_name;
	char	*var_value;
	int		k;

	(*i)++;
	var_len = get_var_name_len(token->value, *i);
	if (var_len == 0)
	{
		dest[*j] = '$';
		(*j)++;
		return ;
	}
	var_name = get_var_name(token->value, *i, var_len);
	if (!var_name)
		return ;
	var_value = find_in_envp(var_name, token->data->env_var);
	if (var_value)
	{
		k = 0;
		while (var_value[k])
			dest[(*j)++] = var_value[k++];
	}
	free(var_name);
	*i += var_len;
}

void	check_if_special_var(t_tokens *token, char *dest, int *i, int *j)
{
	char	*exit_code_str;
	int		k;

	if (token->value[*i] == '$' && token->value[*i + 1] == '?')
	{
		if (g_exit_status == 130)
			exit_code_str = ft_itoa(130);
		else
			exit_code_str = ft_itoa(token->data->exit_code);
		if (!exit_code_str)
			return ;
		k = 0;
		while (exit_code_str[k])
		{
			dest[*j] = exit_code_str[k];
			(*j)++;
			k++;
		}
		free(exit_code_str);
		(*i) += 2;
	}
	else if (token->value[*i] == '$')
	{
		copy_exp_var(token, dest, i, j);
	}
}

int	copy_single_quote(char *src, char *dest, int *i, int *j)
{
	int	start_j;

	start_j = *j;
	(*i)++;
	while (src[*i] && src[*i] != '\'')
	{
		dest[*j] = src[*i];
		(*j)++;
		(*i)++;
	}
	if (src[*i] == '\'')
		(*i)++;
	return (*j - start_j);
}

int	copy_double_quote(t_tokens *token, char *dest, int *i, int *j)
{
	int	start_j;

	start_j = *j;
	(*i)++;
	while (token->value[*i] && token->value[*i] != '\"')
	{
		if (token->value[*i] == '$' && (token->value[*i + 1] != ' '
				&& token->value[*i + 1] != '\0'))
			check_if_special_var(token, dest, i, j);
		else
		{
			dest[*j] = token->value[*i];
			(*j)++;
			(*i)++;
		}
	}
	if (token->value[*i] == '\"')
		(*i)++;
	return (*j - start_j);
}

void	copy_token_value(t_tokens *token, t_data *data)
{
	char	*new_value;
	int		i;
	int		j;

	token->data = data;
	new_value = malloc(sizeof(char) * (calc_token_len(token) + 1));
	if (!new_value)
		return ;
	i = 0;
	j = 0;
	while (token->value[i])
		quotes_handler(token, new_value, &i, &j);
	new_value[j] = '\0';
	free(token->value);
	token->value = new_value;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_token_len.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 12:03:11 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 10:07:34 by nkuydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	clac_signle_quote_len(char *s, int *i)
{
	int	len;

	len = 0;
	(*i)++;
	while (s[*i] && s[*i] != '\'')
	{
		len++;
		(*i)++;
	}
	if (s[*i] == '\'')
		(*i)++;
	return (len);
}

int	calc_var_len(char *s, int *i, t_envp *env_var)
{
	int		var_len;
	char	*var_name;
	char	*var_value;
	int		len;

	(*i)++;
	var_len = get_var_name_len(s, *i);
	if (var_len == 0)
		return (1);
	var_name = get_var_name(s, *i, var_len);
	if (!var_name)
		return (0);
	var_value = find_in_envp(var_name, env_var);
	len = 0;
	if (var_value)
		len = ft_strlen(var_value);
	free(var_name);
	*i += var_len;
	return (len);
}

int	calc_double_quote_len(char *s, int *i, t_envp *env_var)
{
	int	len;

	len = 0;
	(*i)++;
	while (s[*i] && s[*i] != '\"')
	{
		if (s[*i] == '$')
			len += calc_var_len(s, i, env_var);
		else
		{
			len++;
			(*i)++;
		}
	}
	if (s[*i] == '\"')
		(*i)++;
	return (len);
}

int	calc_token_len_special_var_len(t_tokens *token, int *i, t_envp *env_var)
{
	int		len;
	char	*exit_str;

	len = 0;
	if (token->value[*i] == '$' && token->value[*i + 1] == '?')
	{
		*i += 2;
		if (token->data->exit_code)
		{
			exit_str = ft_itoa(token->data->exit_code);
			len += ft_strlen(exit_str);
			free(exit_str);
		}
		else
		{
			len += 1;
		}
	}
	else if (token->value[*i] == '$')
		len += calc_var_len(token->value, i, env_var);
	return (len);
}

int	calc_token_len(t_tokens *token)
{
	int		i;
	int		len;

	len = 0;
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '\'')
			len += clac_signle_quote_len(token->value, &i);
		else if (token->value[i] == '\"')
			len += calc_double_quote_len(token->value, &i,
					token->data->env_var);
		else if (token->value[i] == '$')
			len += calc_token_len_special_var_len
				(token, &i, token->data->env_var);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

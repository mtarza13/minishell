/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:26:38 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/01 06:07:51 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*handle_quote(char *str, int *i, int *sq, int *dq)
{
	char	q_char;
	char	*result;

	q_char = str[*i];
	result = ft_malloc(2, 69);
	result[1] = '\0';
	if (q_char == '\'' && !*dq)
	{
		*sq = !*sq;
		result[0] = q_char;
		(*i)++;
	}
	else if (q_char == '"' && !*sq)
	{
		*dq = !*dq;
		result[0] = q_char;
		(*i)++;
	}
	else
	{
		result[0] = q_char;
		(*i)++;
	}
	return (result);
}

int	calculate_unquoted_length(char *str)
{
	int		len;
	int		i;
	int		in_quotes;
	char	quote_char;

	init_unquote_vars(&len, &i, &in_quotes, &quote_char);
	while (str[i])
	{
		update_quote_state(str[i], &in_quotes, &quote_char);
		if (!in_quotes)
			len++;
		i++;
	}
	return (len);
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		in_quotes;
	char	quote_char;

	if (!str)
		return (NULL);
	result = ft_malloc(calculate_unquoted_length(str) + 1, 69);
	init_remove_vars(&i, &j, &in_quotes, &quote_char);
	while (str[i])
	{
		update_quote_state(str[i], &in_quotes, &quote_char);
		if (!in_quotes || str[i] != quote_char)
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

static char	*append_char(char *result, char c)
{
	char	*tmp;

	tmp = ft_malloc(2, 69);
	tmp[0] = c;
	tmp[1] = '\0';
	return (append_to_result(result, tmp));
}

char	*expand_string(char *str, t_data *data)
{
	char *result;
	char *tmp;
	char *var_value;
	int i;
	int in_single_quote;
	int in_double_quote;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			tmp = handle_quote(str, &i, &in_single_quote, &in_double_quote);
			result = append_to_result(result, tmp);
		}
		else if (str[i] == '$' && should_expand_dollar(str, i, in_single_quote))
		{
			var_value = expand_variable(str, &i, data);
			result = append_to_result(result, var_value);
		}
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
	}
	return (result);
}
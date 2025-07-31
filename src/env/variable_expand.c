/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:56:11 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 06:41:07 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_variable_name(char *str, int *i)
{
	int		start;

	start = *i;
	if (str[*i] == '?' || str[*i] == '$' || ft_isdigit(str[*i]))
	{
		(*i)++;
		return (ft_substr(str, start, 1));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (*i > start)
		return (ft_substr(str, start, *i - start));
	return (NULL);
}

static char	*get_variable_value(char *name, t_data *data)
{
	char	*value;

	if (!name)
		return (ft_strdup(""));
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(data->status));
	if (ft_strcmp(name, "$") == 0)
		return (ft_itoa(getpid()));
	if (ft_isdigit(name[0]))
		return (ft_strdup(""));
	value = get_env_value(name, data);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static int estimate_expansion_size(char *str, t_data *data)
{
	int estimated_size = 0;
	int i = 0;
	
	if (str[0] == '$' && (str[1] == '\'' || str[1] == '"'))
		i = 1;
	
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			int start = i;
			char *name = get_variable_name(str, &i);
			if (name)
			{
				char *value = get_variable_value(name, data);
				estimated_size += ft_strlen(value);
			}
			else
			{
				estimated_size += 1;
				if (i == start)
					i++;
			}
		}
		else if (str[i] == '\'' || str[i] == '"')
			i++;
		else
		{
			estimated_size += 1;
			i++;
		}
	}
	return (estimated_size);
}

char	*expand_variables_advanced(char *str, t_data *data)
{
	char	*result;
	int		i;
	int		j;
	int		max_size;
	t_expand_context ctx;

	if (!str)
		return (NULL);

	max_size = estimate_expansion_size(str, data);
	if (max_size == 0)
		max_size = 1;
	else
		max_size += 1;
	result = ft_malloc(sizeof(char) * max_size, 69);
	ft_memset(result, 0, max_size);
	i = 0;
	j = 0;
	ft_memset(&ctx, 0, sizeof(t_expand_context));

	if (str[0] == '$' && (str[1] == '\'' || str[1] == '"'))
	{
		i = 1;
	}

	while (str[i])
	{
		if (str[i] == '\'' && !ctx.in_double_quote)
		{
			ctx.in_single_quote = !ctx.in_single_quote;
			i++;
			continue;
		}
		if (str[i] == '"' && !ctx.in_single_quote)
		{
			ctx.in_double_quote = !ctx.in_double_quote;
			i++;
			continue;
		}
		if (str[i] == '$' && !ctx.in_single_quote)
		{
			i++;
			char *name = get_variable_name(str, &i);
			char *value = get_variable_value(name, data);
			int value_len = ft_strlen(value);
			int k = 0;
			while (k < value_len && j < max_size - 1)
				result[j++] = value[k++];

		}
		else
		{
			if (j < max_size - 1)
				result[j++] = str[i++];
			else
				break;
		}
	}
	result[j] = '\0';
	return (result);
}

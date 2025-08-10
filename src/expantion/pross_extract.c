/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pross_extract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 00:05:07 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/10 14:54:38 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_env_value(char *str, int *i, t_data *data)
{
	int		start;
	char	*var_value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(data->status));
	}
	if (!ft_isalpha(str[*i]) && str[*i] != '_')
		return (ft_strdup("$"));
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_value = get_env(data, ft_substr(str, start, *i - start));
	if (var_value == NULL)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}

char	*remove_quote(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		s_quote;
	int		d_quote;

	if (!str)
		return (NULL);
	result = ft_malloc(ft_strlen(str) + 1, 1337);
	i = 0;
	j = 0;
	s_quote = 0;
	d_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == '"' && !s_quote)
			d_quote = !d_quote;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	split_count_word(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && is_space(str[i]))
			i++;
		if (str[i])
		{
			count++;
			while (str[i] && !is_space(str[i]))
				i++;
		}
	}
	return (count);
}

char	**split_word(char *str)
{
	char	**word;

	int (count), (i), (start);
	i = 0;
	if (!str || !*str)
		return (NULL);
	count = split_count_word(str);
	if (count == 0)
		return (NULL);
	word = ft_malloc(sizeof(char *) * (count + 1), 1337);
	count = 0;
	while (str[i])
	{
		while (str[i] && is_space(str[i]))
			i++;
		if (str[i])
		{
			start = i;
			while (str[i] && !is_space(str[i]))
				i++;
			word[count++] = ft_substr(str, start, i - start);
		}
	}
	word[count] = NULL;
	return (word);
}

char	*expand_string(char *arg, t_data *data)
{
	char *(result);
	int (i), s_quote = 0, d_quote = 0;
	i = 0;
	if (!arg)
		return (NULL);
	result = ft_strdup("");
	if (!arg)
		return (NULL);
	while (arg[i])
	{
		if (arg[i] == '\'' && !d_quote)
		{
			s_quote = !s_quote;
			result = add_char(result, arg[i++]);
		}
		else if (arg[i] == '"' && !s_quote)
			(1) && (d_quote = !d_quote) && \
			(result = add_char(result, arg[i++]));
		else if (arg[i] == '$' && !s_quote)
			result = ft_strjoin(result, expand_env_value(arg, &i, data));
		else
			result = add_char(result, arg[i++]);
	}
	return (result);
}

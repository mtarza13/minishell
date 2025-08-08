/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 00:05:17 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/09 00:05:19 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_quote(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

int	count_total_word(char **arg, t_data *data)
{
	char	**split_words;

	int (i), (j), (count);
	i = 0;
	count = 0;
	while (arg[i])
	{
		if (has_quote(arg[i]))
			count++;
		else
		{
			split_words = split_word(remove_quote(expand_string(arg[i], data)));
			if (split_words)
			{
				j = 0;
				while (split_words[j])
				{
					count++;
					j++;
				}
			}
		}
		i++;
	}
	return (count);
}

char	**expand_arg_array(char **arg, t_data *data)
{
	char	**result;
	char	**split_words;

	int (idx), i = 0, j = 0, count = 0;
	idx = 0;
	count = count_total_word(arg, data);
	result = ft_malloc(sizeof(char *) * (count + 1), 1337);
	while (arg[i])
	{
		if (has_quote(arg[i]))
			result[idx++] = remove_quote(expand_string(arg[i], data));
		else
		{
			split_words = split_word(remove_quote(expand_string(arg[i], data)));
			j = 0;
			while (split_words[j])
			{
				result[idx++] = ft_strdup(split_words[j]);
				j++;
			}
		}
		i++;
	}
	result[idx] = NULL;
	return (result);
}

char	*expand_arg_list(char **args, t_data *data)
{
	char	**expanded;
	char	*result;
	int		i;

	i = 0;
	expanded = expand_arg_array(args, data);
	if (!expanded)
		return (NULL);
	result = ft_strdup("");
	while (expanded[i])
	{
		if (i > 0)
			result = ft_strjoin(result, " ");
		result = ft_strjoin(result, expanded[i]);
		i++;
	}
	return (result);
}

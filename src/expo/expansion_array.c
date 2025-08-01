/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 03:17:57 by yasserbarhd       #+#    #+#             */
/*   Updated: 2025/08/01 06:10:35 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	add_split_words(char **final_args, int k, char **split)
{
	int	j;

	j = 0;
	while (split[j])
	{
		final_args[k] = remove_quotes(split[j]);
		k++;
		j++;
	}
	return (k);
}

static int	add_expanded_word(char **final_args, int k, char *expanded)
{
	if (expanded && *expanded)
	{
		final_args[k] = remove_quotes(expanded);
		k++;
	}
	return (k);
}

void	fill_final_args(char **args, t_data *data, char **final_args)
{
	int		i;
	int		k;
	char	*expanded;
	char	**split;

	i = 0;
	k = 0;
	while (args[i])
	{
		expanded = expand_string(args[i], data);
		split = split_expanded_string(expanded);
		if (split)
			k = add_split_words(final_args, k, split);
		else
			k = add_expanded_word(final_args, k, expanded);
		i++;
	}
	final_args[k] = NULL;
}

char	**expand_arg_array(char **args, t_data *data)
{
	char	**final_args;
	int		total_count;

	if (!args)
		return (NULL);
	total_count = count_total_words(args, data);
	final_args = ft_malloc(sizeof(char *) * (total_count + 1), 69);
	fill_final_args(args, data, final_args);
	return (final_args);
}

char	*expand_arg_list(char *str, t_data *data)
{
	char	*expanded;

	if (!str)
		return (NULL);
	expanded = expand_string(str, data);
	return (remove_quotes(expanded));
}

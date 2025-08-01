/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:49:07 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/01 06:11:08 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_remove_vars(int *i, int *j, int *in_quotes, char *quote_char)
{
	*i = 0;
	*j = 0;
	*in_quotes = 0;
	*quote_char = 0;
}

void	init_unquote_vars(int *len, int *i, int *in_quotes, char *quote_char)
{
	*len = 0;
	*i = 0;
	*in_quotes = 0;
	*quote_char = 0;
}

int	count_total_words(char **args, t_data *data)
{
	int		total_count;
	int		i;
	char	*expanded;
	char	**split;

	total_count = 0;
	i = 0;
	while (args[i])
	{
		expanded = expand_string(args[i], data);
		split = split_expanded_string(expanded);
		if (split)
			total_count += count_split_words(split);
		else if (expanded && *expanded)
			total_count++;
		i++;
	}
	return (total_count);
}

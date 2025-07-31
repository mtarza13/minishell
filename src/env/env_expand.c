/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:55:49 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 21:18:21 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**add_word_to_array(char **array, char *word)
{
	int		count;
	char	**new_array;
	int		i;

	count = 0;
	if (array)
	{
		while (array[count])
			count++;
	}
	new_array = ft_malloc(sizeof(char *) * (count + 2), 69);
	if (!new_array)
		return (NULL); 
	i = 0;
	while (i < count)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[count] = ft_strdup(word);
	new_array[count + 1] = NULL; 
	return (new_array);
}

char	**expand_args_professional(char **args, t_data *data)
{
	char	**final_args;
	char	*expanded_str;
	char	**split_words;
	int		i;
	int		j;

	if (!args)
		return (NULL);
	final_args = NULL;
	i = 0;
	while (args[i])
	{
		expanded_str = expand_variables_advanced(args[i], data);
		if (!expanded_str)
		{
			i++;
			continue ;
		}
		
		if (ft_strchr(args[i], '\'') == NULL && ft_strchr(args[i], '"') == NULL
			&& *expanded_str != '\0')
		{
			split_words = ft_split(expanded_str, ' ');
			j = 0;
			while (split_words && split_words[j])
			{
				final_args = add_word_to_array(final_args, split_words[j]);
				j++;
			}
		}
		else
		{
		
			final_args = add_word_to_array(final_args, expanded_str);
		}
		// free(expanded_str);
		i++;
	}
	return (final_args);
}

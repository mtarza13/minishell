/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_word_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:26:17 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/01 05:10:44 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_quote_state(char c, int *in_quotes, char *quote_char)
{
	if (!*in_quotes && (c == '\'' || c == '"'))
	{
		*in_quotes = 1;
		*quote_char = c;
	}
	else if (*in_quotes && c == *quote_char)
	{
		*in_quotes = 0;
		*quote_char = 0;
	}
}

int	count_words_after_expansion(char *str)
{
	int		count;
	int		in_word;
	int		in_quotes;
	int		i;
	char	quote_char;

	count = 0;
	in_word = 0;
	in_quotes = 0;
	quote_char = 0;
	i = 0;
	while (str[i])
	{
		update_quote_state(str[i], &in_quotes, &quote_char);
		if (!in_quotes && is_ifs_char(str[i]))
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

int	move_to_next_word(char *str, int *start, int *end)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = 0;
	*start = *end;
	while (str[*start] && is_ifs_char(str[*start]))
		(*start)++;
	if (!str[*start])
		return (0);
	*end = *start;
	while (str[*end])
	{
		update_quote_state(str[*end], &in_quotes, &quote_char);
		if (!in_quotes && is_ifs_char(str[*end]))
			break ;
		(*end)++;
	}
	return (1);
}

char	*extract_word(char *str, int *start, int *end)
{
	if (!move_to_next_word(str, start, end))
		return (NULL);
	return (ft_substr(str, *start, *end - *start));
}

char	**split_expanded_string(char *expanded_str)
{
	char	**words;
	int		word_count;
	int		start;
	int		end;
	int		i;

	if (!expanded_str || !*expanded_str)
		return (NULL);
	word_count = count_words_after_expansion(expanded_str);
	if (word_count == 0)
		return (NULL);
	words = ft_malloc(sizeof(char *) * (word_count + 1), 69);
	start = 0;
	end = 0;
	i = 0;
	while (i < word_count)
	{
		words[i] = extract_word(expanded_str, &start, &end);
		if (!words[i])
			break ;
		i++;
	}
	words[i] = NULL;
	return (words);
}

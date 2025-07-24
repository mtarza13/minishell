
#include "../../include/minishell.h"

char	*remove_quotes_advanced(char *str)
{
	char	*result;
	int		capacity;
	int		i;
	int		j;
	char	quote;

	if (!str)
		return (NULL);
	capacity = ft_strlen(str) + 1;
	result = malloc(capacity);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				result[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else if (str[i] == '\\' && str[i + 1])
		{
			i++;
			result[j++] = str[i++];
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static int	count_words(char *str)
{
	int	word_count;
	int	in_word;
	int	i;

	word_count = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			word_count++;
		}
		i++;
	}
	return (word_count);
}

char	**split_unquoted_words(char *str)
{
	char	**words;
	int		word_count;
	int		word_idx;
	int		i;
	int		start;

	if (!str || !*str)
		return (NULL);
	word_count = count_words(str);
	if (word_count == 0)
		return (NULL);
	words = malloc(sizeof(char*) * (word_count + 1));
	if (!words)
		return (NULL);
	word_idx = 0;
	i = 0;
	while (word_idx < word_count && str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (!str[i])
			break ;
		start = i;
		while (str[i] && !ft_isspace(str[i]))
			i++;
		words[word_idx] = ft_substr(str, start, i - start);
		if (!words[word_idx])
		{
			ft_free_array(words);
			return (NULL);
		}
		word_idx++;
	}
	words[word_idx] = NULL;
	return (words);
}
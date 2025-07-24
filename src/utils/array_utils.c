
#include "../../include/minishell.h"

static int	expand_single_arg(char **args, int i, t_env *env,
			char ***result, int *result_count, int *result_capacity)
{
	int		has_quotes;
	char	*expanded;
	char	*unquoted;
	char	**split_words;
	int		split_count;
	int		j;

	has_quotes = (ft_strchr(args[i], '"') != NULL
		|| ft_strchr(args[i], '\'') != NULL);
	expanded = expand_variables_advanced(args[i], env);
	if (!expanded)
		expanded = ft_strdup("");
	unquoted = remove_quotes_advanced(expanded);
	free(expanded);
	if (!unquoted)
		unquoted = ft_strdup("");
	if (has_quotes)
	{
		if (*result_count >= *result_capacity - 1)
		{
			*result_capacity *= 2;
			*result = realloc(*result, sizeof(char*) * (*result_capacity));
			if (!*result)
				return (0);
		}
		(*result)[(*result_count)++] = unquoted;
	}
	else
	{
		split_words = split_unquoted_words(unquoted);
		if (split_words)
		{
			split_count = 0;
			while (split_words[split_count])
				split_count++;
			while (*result_count + split_count >= *result_capacity)
			{
				*result_capacity *= 2;
				*result = realloc(*result, sizeof(char*) * (*result_capacity));
				if (!*result)
					return (0);
			}
			j = 0;
			while (j < split_count)
			{
				(*result)[(*result_count)++] = split_words[j];
				j++;
			}
			free(split_words);
			free(unquoted);
		}
		else
		{
			if (*unquoted)
			{
				if (*result_count >= *result_capacity - 1)
				{
					*result_capacity *= 2;
					*result = realloc(*result, sizeof(char*) * (*result_capacity));
					if (!*result)
						return (0);
				}
				(*result)[(*result_count)++] = unquoted;
			}
			else
				free(unquoted);
		}
	}
	return (1);
}

char	**expand_args_professional(char **args, t_env *env)
{
	char	**result;
	int		original_count;
	int		result_capacity;
	int		result_count;
	int		i;

	if (!args)
		return (NULL);
	original_count = 0;
	while (args[original_count])
		original_count++;
	result_capacity = original_count * 4 + 10;
	result = malloc(sizeof(char*) * result_capacity);
	if (!result)
		return (NULL);
	result_count = 0;
	i = 0;
	while (i < original_count)
	{
		if (!expand_single_arg(args, i, env, &result, &result_count,
				&result_capacity))
			return (NULL);
		i++;
	}
	result[result_count] = NULL;
	return (result);
}
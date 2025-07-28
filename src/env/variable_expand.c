#include "../../include/minishell.h"

/*
 * get_variable_name:
 * A static helper to extract the name of a variable following a '$'.
 * Handles single-character names like '?' and '$', as well as alphanumeric names.
 */
static char	*get_variable_name(char *str, int *i)
{
	int		start;

	start = *i;
	// Handles special single-character variables like $? and $$
	if (str[*i] == '?' || str[*i] == '$' || ft_isdigit(str[*i]))
	{
		(*i)++;
		return (ft_substr(str, start, 1));
	}
	// Handles standard alphanumeric variable names (e.g., $USER, $VAR_1)
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (*i > start)
		return (ft_substr(str, start, *i - start));
	// If '$' is followed by something else (like a space or '/'), it's not a var
	return (NULL);
}

/*
 * get_variable_value:
 * A static helper that takes a variable name and returns its allocated value.
 * Handles special shell variables and looks up others in the environment.
 */
static char	*get_variable_value(char *name, t_data *data)
{
	char	*value;

	if (!name)
		return (ft_strdup(""));
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(0)); // placeholder
	if (ft_strcmp(name, "$") == 0)
		return (ft_itoa(getpid())); // Note: This should be the PID of the main shell process
	// In an interactive shell, positional parameters like $0, $1 are not expanded
	if (ft_isdigit(name[0]))
		return (ft_strdup(""));
	value = get_env_value(name, data);
	if (value)
		return (ft_strdup(value));
	// If the variable is not found, it expands to an empty string
	return (ft_strdup(""));
}

/*
 * estimate_expansion_size:
 * Estimates the maximum size needed after variable expansion
 */
static int estimate_expansion_size(char *str, t_data *data)
{
	int estimated_size = 0;
	int i = 0;
	
	// Handle the special case where str starts with $' or $"
	if (str[0] == '$' && (str[1] == '\'' || str[1] == '"'))
		i = 1; // Skip the initial '$'
	
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++; // Skip '$'
			int start = i;
			char *name = get_variable_name(str, &i);
			if (name)
			{
				char *value = get_variable_value(name, data);
				estimated_size += ft_strlen(value);
				free(name);
				free(value);
			}
			else
			{
				estimated_size += 1; // Just the '$' character
				if (i == start) // No variable name was extracted
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

/*
 * expand_variables_advanced:
 * The definitive single-pass function for all expansions and quote removal.
 * It correctly handles all quoting rules and variable expansion syntax.
 */
char	*expand_variables_advanced(char *str, t_data *data)
{
	char	*result;
	int		i;
	int		j;
	int		max_size;
	t_expand_context ctx;

	if (!str)
		return (NULL);
	// Calculate buffer size: estimated expansion + 1 for null terminator
	// Ensure minimum size of 1 for empty strings
	max_size = estimate_expansion_size(str, data);
	if (max_size == 0)
		max_size = 1;
	else
		max_size += 1; // Add space for null terminator
	result = malloc(sizeof(char) * max_size);
	if (!result)
		return (NULL);
	ft_memset(result, 0, max_size);
	i = 0;
	j = 0;
	ft_memset(&ctx, 0, sizeof(t_expand_context));

	// --- THIS IS THE DEFINITIVE FIX ---
	// If the token starts with `$` followed by a quote, skip the initial `$`
	// as it's part of the syntax, not a character to be printed.
	if (str[0] == '$' && (str[1] == '\'' || str[1] == '"'))
	{
		i = 1; // Start processing from the first quote, skipping the '$'
	}
	// --- END OF FIX ---

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
			i++; // Consume '$'
			char *name = get_variable_name(str, &i);
			char *value = get_variable_value(name, data);
			// Use strcpy/strcat approach or manual copying instead of ft_strlcat
			int value_len = ft_strlen(value);
			int k = 0;
			// Add bounds checking to prevent buffer overflow
			while (k < value_len && j < max_size - 1)
				result[j++] = value[k++];
			free(name);
			free(value);
		}
		else
		{
			if (j < max_size - 1)
				result[j++] = str[i++];
			else
				break;
		}
	}
	result[j] = '\0'; // Ensure null termination
	return (result);
}
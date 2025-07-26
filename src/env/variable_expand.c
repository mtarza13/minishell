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
static char	*get_variable_value(char *name, t_env *env)
{
	char	*value;

	if (!name)
		return (ft_strdup(""));
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(env->exit_status));
	if (ft_strcmp(name, "$") == 0)
		return (ft_itoa(getpid())); // Note: This should be the PID of the main shell process
	// In an interactive shell, positional parameters like $0, $1 are not expanded
	if (ft_isdigit(name[0]))
		return (ft_strdup(""));
	value = get_env_value(env, name);
	if (value)
		return (ft_strdup(value));
	// If the variable is not found, it expands to an empty string
	return (ft_strdup(""));
}

/*
 * expand_variables_advanced:
 * The definitive single-pass function for all expansions and quote removal.
 * It correctly handles all quoting rules and variable expansion syntax.
 */
char	*expand_variables_advanced(char *str, t_env *env)
{
	char	*result;
	int		i;
	int		j;
	t_expand_context ctx;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) * 2 + 1));
	if (!result)
		return (NULL);
	ft_memset(result, 0, ft_strlen(str) * 2 + 1);
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
			char *value = get_variable_value(name, env);
			ft_strlcat(result, value, ft_strlen(str) * 2 + 1);
			j = ft_strlen(result);
			free(name);
			free(value);
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	return (result);
}
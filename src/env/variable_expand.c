
#include "../../include/minishell.h"

static char	*get_var_name_complete(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (str[*i] == '$')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	if (str[*i] == '0')
	{
		(*i)++;
		return (ft_strdup("0"));
	}
	if (ft_isdigit(str[*i]))
	{
		(*i)++;
		name = malloc(2);
		if (!name)
			return (NULL);
		name[0] = str[start];
		name[1] = '\0';
		return (name);
	}
	if (str[*i] == '{')
	{
		(*i)++;
		start = *i;
		while (str[*i] && str[*i] != '}')
			(*i)++;
		if (str[*i] == '}')
		{
			len = *i - start;
			(*i)++;
			if (len == 0)
				return (ft_strdup(""));
			name = ft_substr(str, start, len);
			return (name);
		}
		*i = start - 1;
		return (NULL);
	}
	if (ft_isalnum(str[*i]) || str[*i] == '_')
	{
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;
		len = *i - start;
		name = ft_substr(str, start, len);
		return (name);
	}
	return (NULL);
}

static char	*expand_variable_value(char *var_name, t_env *env)
{
	char	*value;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(env->exit_status));
	if (ft_strcmp(var_name, "$") == 0)
		return (ft_itoa(getpid()));
	if (ft_strcmp(var_name, "0") == 0)
		return (ft_strdup("minishell"));
	if (ft_isdigit(var_name[0]) && ft_strlen(var_name) == 1)
		return (ft_strdup(""));
	value = get_env_value(env, var_name);
	return (value ? ft_strdup(value) : ft_strdup(""));
}

static int	handle_variable_expansion(char *str, int *i,
			t_expand_context *ctx, char **result, int *result_len,
			int *capacity, t_env *env)
{
	char	*var_name;
	char	*var_value;
	int		j;

	if (str[*i] == '$' && !ctx->in_single_quote)
	{
		if (*i > 0 && str[*i - 1] == '\\')
		{
			*result = append_char_dynamic(*result, str[*i], result_len,
					capacity);
			(*i)++;
			return (1);
		}
		if (!str[*i + 1])
		{
			*result = append_char_dynamic(*result, str[*i], result_len,
					capacity);
			(*i)++;
			return (1);
		}
		(*i)++;
		var_name = get_var_name_complete(str, i);
		if (var_name)
		{
			var_value = expand_variable_value(var_name, env);
			if (var_value)
			{
				j = 0;
				while (var_value[j])
				{
					*result = append_char_dynamic(*result, var_value[j],
							result_len, capacity);
					j++;
				}
				free(var_value);
			}
			free(var_name);
		}
		else
			*result = append_char_dynamic(*result, '$', result_len, capacity);
		return (1);
	}
	return (0);
}

static int	handle_escape_and_quotes(char *str, int *i,
			t_expand_context *ctx, char **result, int *result_len,
			int *capacity)
{
	if (ctx->escape_next)
	{
		*result = append_char_dynamic(*result, str[*i], result_len, capacity);
		ctx->escape_next = 0;
		(*i)++;
		return (1);
	}
	if (str[*i] == '\\' && !ctx->in_single_quote)
	{
		if (str[*i + 1] && (str[*i + 1] == '$' || str[*i + 1] == '"'
				|| str[*i + 1] == '\\' || str[*i + 1] == '\n'
				|| (ctx->in_double_quote && str[*i + 1] == '`')))
		{
			ctx->escape_next = 1;
			(*i)++;
			return (1);
		}
		else if (!ctx->in_double_quote)
		{
			*result = append_char_dynamic(*result, str[*i], result_len,
					capacity);
			(*i)++;
			return (1);
		}
	}
	if (str[*i] == '\'' && !ctx->in_double_quote)
	{
		ctx->in_single_quote = !ctx->in_single_quote;
		*result = append_char_dynamic(*result, str[*i], result_len, capacity);
		(*i)++;
		return (1);
	}
	if (str[*i] == '"' && !ctx->in_single_quote)
	{
		ctx->in_double_quote = !ctx->in_double_quote;
		*result = append_char_dynamic(*result, str[*i], result_len, capacity);
		ctx->preserve_empty = 1;
		(*i)++;
		return (1);
	}
	return (0);
}

char	*expand_variables_advanced(char *str, t_env *env)
{
	char				*result;
	int					capacity;
	int					result_len;
	int					i;
	t_expand_context	ctx;

	if (!str)
		return (NULL);
	capacity = ft_strlen(str) * 2 + 100;
	result = malloc(capacity);
	if (!result)
		return (NULL);
	result[0] = '\0';
	result_len = 0;
	i = 0;
	ft_memset(&ctx, 0, sizeof(t_expand_context));
	while (str[i])
	{
		if (handle_escape_and_quotes(str, &i, &ctx, &result, &result_len,
				&capacity))
			continue ;
		if (handle_variable_expansion(str, &i, &ctx, &result, &result_len,
				&capacity, env))
			continue ;
		result = append_char_dynamic(result, str[i], &result_len, &capacity);
		i++;
	}
	return (result);
}
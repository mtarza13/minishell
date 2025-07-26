#include "../../include/minishell.h"

/*
 * get_var_name_complete:
 * A static helper function to extract the name of a variable following a '$'.
 * Handles cases like $VAR, ${VAR}, $?, $$, and $0.
 */
static char *get_var_name_complete(char *str, int *i)
{
    int     start;
    int     len;
    char    *name;

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

/*
 * expand_variable_value:
 * A static helper that takes a variable name and returns its value from the
 * environment or as a special shell variable.
 */
static char *expand_variable_value(char *var_name, t_env *env)
{
    char *value;

    if (!var_name)
        return (ft_strdup(""));
    if (ft_strcmp(var_name, "?") == 0)
        return (ft_itoa(env->exit_status));
    if (ft_strcmp(var_name, "$") == 0)
        return (ft_itoa(getpid())); 
    if (ft_strcmp(var_name, "0") == 0)
        return (ft_strdup("minishell"));
    value = get_env_value(env, var_name);
    return (value ? ft_strdup(value) : ft_strdup(""));
}

/*
 * expand_variables_advanced:
 * The main expansion function. It processes a string, expands variables,
 * handles quotes, and manages the $"..." syntax, all in a single pass.
 * This function is the single source of truth for expansion.
 */
char *expand_variables_advanced(char *str, t_env *env)
{
    char                *result;
    int                 capacity;
    int                 result_len;
    int                 i;
    t_expand_context    ctx;
    char                *var_name;
    char                *var_value;

    if (!str)
        return (NULL);
    capacity = ft_strlen(str) * 2 + 100;
    result = malloc(capacity);
    if (!result) return (NULL);
    result[0] = '\0';
    result_len = 0;
    i = 0;
    ft_memset(&ctx, 0, sizeof(t_expand_context));

    while (str[i])
    {
        if (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '"'))
        {
            char quote_char = str[++i];
            i++;
            while (str[i] && str[i] != quote_char)
                result = append_char_dynamic(result, str[i++], &result_len, &capacity);
            if (str[i] == quote_char) i++;
            continue;
        }
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
            i++;
            var_name = get_var_name_complete(str, &i);
            if (var_name)
            {
                var_value = expand_variable_value(var_name, env);
                if (var_value)
                {
                    int j = 0;
                    while (var_value[j])
                        result = append_char_dynamic(result, var_value[j++], &result_len, &capacity);
                    free(var_value);
                }
                free(var_name);
            }
            else
                result = append_char_dynamic(result, '$', &result_len, &capacity);
        }
        else
            result = append_char_dynamic(result, str[i++], &result_len, &capacity);
    }
    return (result);
}

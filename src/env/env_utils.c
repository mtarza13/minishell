
#include "../../include/minishell.h"

char	*get_env_value(t_env *env, char *name)
{
	int	len;
	int	i;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = -1;
	while (++i < env->count)
	{
		if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0
			&& env->envp[i][len] == '=')
			return (env->envp[i] + len + 1);
	}
	return (NULL);
}

static int	update_existing_env(t_env *env, char *name, char *value, int len)
{
	int		i;
	char	*new_var;

	i = -1;
	while (++i < env->count)
	{
		if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0
			&& (env->envp[i][len] == '=' || env->envp[i][len] == '\0'))
		{
			free(env->envp[i]);
			if (value)
			{
				new_var = ft_strjoin_free(ft_strdup(name), ft_strdup("="), 0, 0);
				env->envp[i] = ft_strjoin_free(new_var, ft_strdup(value), 1, 0);
			}
			else
				env->envp[i] = ft_strdup(name);
			return (1);
		}
	}
	return (0);
}

static int	expand_env_capacity(t_env *env)
{
	int	i;

	if (env->count >= env->capacity - 1)
	{
		env->capacity *= 2;
		env->envp = realloc(env->envp, sizeof(char*) * env->capacity);
		if (!env->envp)
			return (0);
		i = env->count;
		while (i < env->capacity)
			env->envp[i++] = NULL;
	}
	return (1);
}

int	set_env_value(t_env *env, char *name, char *value)
{
	int		len;
	char	*new_var;

	if (!env || !name)
		return (0);
	len = ft_strlen(name);
	if (update_existing_env(env, name, value, len))
		return (1);
	if (!expand_env_capacity(env))
		return (0);
	if (value)
	{
		new_var = ft_strjoin_free(ft_strdup(name), ft_strdup("="), 0, 0);
		env->envp[env->count] = ft_strjoin_free(new_var, ft_strdup(value), 1, 0);
	}
	else
		env->envp[env->count] = ft_strdup(name);
	env->count++;
	return (1);
}

int	unset_env_value(t_env *env, char *name)
{
	int	len;
	int	i;
	int	j;

	if (!env || !name)
		return (0);
	len = ft_strlen(name);
	i = -1;
	while (++i < env->count)
	{
		if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0
			&& (env->envp[i][len] == '=' || env->envp[i][len] == '\0'))
		{
			free(env->envp[i]);
			j = i;
			while (j < env->count - 1)
			{
				env->envp[j] = env->envp[j + 1];
				j++;
			}
			env->envp[env->count - 1] = NULL;
			env->count--;
			return (1);
		}
	}
	return (0);
}
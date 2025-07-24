
#include "../../include/minishell.h"

static void	sort_env_for_export(char **env_array, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (env_array[j] && env_array[j + 1]
				&& ft_strcmp(env_array[j], env_array[j + 1]) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_format(char *env_var)
{
	char	*equals;

	equals = ft_strchr(env_var, '=');
	if (equals)
	{
		*equals = '\0';
		printf("declare -x %s=\"%s\"\n", env_var, equals + 1);
		*equals = '=';
	}
	else
		printf("declare -x %s\n", env_var);
}

static int	export_without_args(t_env *env)
{
	char	**sorted_env;
	int		i;
	int		j;

	sorted_env = malloc(sizeof(char*) * (env->count + 1));
	if (!sorted_env)
		return (1);
	i = 0;
	j = 0;
	while (i < env->count)
	{
		if (env->envp[i])
			sorted_env[j++] = ft_strdup(env->envp[i]);
		i++;
	}
	sorted_env[j] = NULL;
	sort_env_for_export(sorted_env, j);
	i = 0;
	while (i < j)
	{
		if (sorted_env[i])
			print_export_format(sorted_env[i]);
		i++;
	}
	ft_free_array(sorted_env);
	return (0);
}

static int	export_with_value(t_env *env, char *name, char *value)
{
	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (!set_env_value(env, name, value))
	{
		ft_putstr_fd("minishell: export: failed to set variable\n", 2);
		return (1);
	}
	return (0);
}

static int	export_without_value(t_env *env, char *name)
{
	char	*existing_value;

	if (!is_valid_identifier(name))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	existing_value = get_env_value(env, name);
	if (existing_value)
	{
		if (!set_env_value(env, name, existing_value))
		{
			ft_putstr_fd("minishell: export: failed to export variable\n", 2);
			return (1);
		}
	}
	else
	{
		if (!set_env_value(env, name, NULL))
		{
			ft_putstr_fd("minishell: export: failed to export variable\n", 2);
			return (1);
		}
	}
	return (0);
}

int	builtin_export(char **args, t_env *env)
{
	int		exit_status;
	int		i;
	char	*equals;

	if (!args[1])
		return (export_without_args(env));
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		equals = ft_strchr(args[i], '=');
		if (equals)
		{
			*equals = '\0';
			exit_status |= export_with_value(env, args[i], equals + 1);
			*equals = '=';
		}
		else
			exit_status |= export_without_value(env, args[i]);
		i++;
	}
	return (exit_status);
}
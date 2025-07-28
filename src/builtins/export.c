/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:32:06 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 15:57:22 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// static void	sort_env_for_export(char **env_array, int count)
// {
// 	int		i;
// 	int		j;
// 	char	*temp;

// 	i = 0;
// 	while (i < count - 1)
// 	{
// 		j = 0;
// 		while (j < count - i - 1)
// 		{
// 			if (env_array[j] && env_array[j + 1]
// 				&& ft_strcmp(env_array[j], env_array[j + 1]) > 0)
// 			{
// 				temp = env_array[j];
// 				env_array[j] = env_array[j + 1];
// 				env_array[j + 1] = temp;
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// }

// static void	print_export_format(char *env_var)
// {
// 	char	*equals;

// 	equals = ft_strchr(env_var, '=');
// 	if (equals)
// 	{
// 		*equals = '\0';
// 		printf("declare -x %s=\"%s\"\n", env_var, equals + 1);
// 		*equals = '=';
// 	}
// 	else
// 		printf("declare -x %s\n", env_var);
// }

// static int	export_without_args(t_env *env)
// {
// 	char	**sorted_env;
// 	int		i;
// 	int		j;

// 	sorted_env = malloc(sizeof(char*) * (env->count + 1));
// 	if (!sorted_env)
// 		return (1);
// 	i = 0;
// 	j = 0;
// 	while (i < env->count)
// 	{
// 		if (env->envp[i])
// 			sorted_env[j++] = ft_strdup(env->envp[i]);
// 		i++;
// 	}
// 	sorted_env[j] = NULL;
// 	sort_env_for_export(sorted_env, j);
// 	i = 0;
// 	while (i < j)
// 	{
// 		if (sorted_env[i])
// 			print_export_format(sorted_env[i]);
// 		i++;
// 	}
// 	ft_free_array(sorted_env);
// 	return (0);
// }

// static int	export_with_value(t_env *env, char *name, char *value)
// {
// 	if (!is_valid_identifier(name))
// 	{
// 		ft_putstr_fd("minishell: export: `", 2);
// 		ft_putstr_fd(name, 2);
// 		ft_putstr_fd("': not a valid identifier\n", 2);
// 		return (1);
// 	}
// 	if (!set_env_value(env, name, value))
// 	{
// 		ft_putstr_fd("minishell: export: failed to set variable\n", 2);
// 		return (1);
// 	}
// 	return (0);
// }

// static int	export_without_value(t_env *env, char *name)
// {
// 	char	*existing_value;

// 	if (!is_valid_identifier(name))
// 	{
// 		ft_putstr_fd("minishell: export: `", 2);
// 		ft_putstr_fd(name, 2);
// 		ft_putstr_fd("': not a valid identifier\n", 2);
// 		return (1);
// 	}
// 	existing_value = get_env_value(env, name);
// 	if (existing_value)
// 	{
// 		if (!set_env_value(env, name, existing_value))
// 		{
// 			ft_putstr_fd("minishell: export: failed to export variable\n", 2);
// 			return (1);
// 		}
// 	}
// 	else
// 	{
// 		if (!set_env_value(env, name, NULL))
// 		{
// 			ft_putstr_fd("minishell: export: failed to export variable\n", 2);
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

int	is_env_char(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '_');
}

void	update_env_node(t_env *env, char *key, char *value)
{
	int (v_len);
	v_len = ft_strlen(key);
	while (env)
	{
		if (!ft_strncmp(env->key, key, v_len))
		{
			env->value = value;
			return ;
		}
		env = env->next;
	}
}

static void	string_check(char **args, int i, int j, int *f, t_env *env)
{
	while (args[i][j])
	{
		if (!is_env_char(args[i][j]))
		{
			if (j != 0 && args[i][j] == '=')
			{
				args[i][j] = '\0';
				if (!get_env_value(env, args[i]))
					add_env_node(&env, new_env_node \
					(ft_strdup(args[i]), ft_strdup(&args[i][j + 1])));
				else
					update_env_node(env, args[i], \
					ft_strdup(&args[i][j + 1]));
			}
			else
			{
				// print_error(args[i], 6);
				// data->status = 1;
				*f = 1;
			}
			break ;
		}
		j++;
	}	
}

// int	builtin_export(char **args, t_env *env)
// {
// 	int		exit_status;
// 	int		i;
// 	char	*equals;

// 	if (!args[1])
// 		return (export_without_args(env));
// 	exit_status = 0;
// 	i = 1;
// 	while (args[i])
// 	{
// 		if (!((args[i][0] >= 'A' && args[i][0] <= 'Z') ||
// 			(args[i][0] >= 'a' && args[i][0] <= 'z') ||
// 			(args[i][0] == '_')))
// 		{
// 			i++;
// 			f = 1;
// 			cmd->data->status = 1;
// 			continue ;
// 		}
// 		equals = ft_strchr(args[i], '=');
// 		if (equals)
// 		{
// 			*equals = '\0';
// 			exit_status |= export_with_value(env, args[i], equals + 1);
// 			*equals = '=';
// 		}
// 		else
// 			exit_status |= export_without_value(env, args[i]);
// 		i++;
// 	}
// 	return (exit_status);
// }

static void	print_envp(t_env **env_arr, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		printf("declare -x %s=\"%s\"\n", env_arr[i]->key, env_arr[i]->value);
}

static void	bubble_sort(t_env **env_arr, int count)
{
	t_env *(tmp);
	int (i), j = 0;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_arr[j]->key, env_arr[j + 1]->key) > 0)
			{
				tmp = env_arr[j];
				env_arr[j] = env_arr[j + 1];
				env_arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}	
}

void	print_sorted_env(t_env *env)
{
	t_env *(tmp), **(env_arr);
	int (i), count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	env_arr = ft_malloc(sizeof(t_env *) * count, 32);
	tmp = env;
	i = 0;
	while (tmp)
	{
		env_arr[i++] = tmp;
		tmp = tmp->next;
	}
	bubble_sort(env_arr, count);
	print_envp(env_arr, count);
}

int	builtin_export(char **args, t_env *env)
{
	int (i), j = 0, flag = 0;
	i = 1;
	if (!args[i] || !args[i][0])
		return (print_sorted_env(env), 0);
	while (args[i])
	{
		if (!((args[i][0] >= 'A' && args[i][0] <= 'Z') ||
			(args[i][0] >= 'a' && args[i][0] <= 'z') ||
			(args[i][0] == '_')))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			i++;
			flag = 1;
			continue ;
		}
		string_check(args, i, j, &flag, env);
		i++;
	}
	return (flag);
}
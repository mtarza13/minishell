/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:32:06 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/30 23:56:37 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static void	string_check(char **args, int i, int j, int *f, t_data *data)
{
	while (args[i][j])
	{
		if (!is_env_char(args[i][j]))
		{
			if (j != 0 && args[i][j] == '=')
			{
				args[i][j] = '\0';
				if (!get_env_value(args[i], data))
					add_env_node(&data->env, new_env_node \
					(ft_strdup(args[i]), ft_strdup(&args[i][j + 1])));
				else
					update_env_node(data->env, args[i], \
					ft_strdup(&args[i][j + 1]));
			}
			else
			{
				// print_error(args[i], 6);
				data->status = 1;
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

void	print_sorted_env(t_data *data)
{
	t_env *(tmp), **(env_arr);
	int (i), count = 0;
	tmp = data->env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	env_arr = ft_malloc(sizeof(t_env *) * count, 32);
	tmp = data->env;
	i = 0;
	while (tmp)
	{
		env_arr[i++] = tmp;
		tmp = tmp->next;
	}
	bubble_sort(env_arr, count);
	print_envp(env_arr, count);
}

int	builtin_export(char **args, t_data *data)
{
	int (i), j = 0, flag = 0;
	i = 1;
	if (!args[i] || !args[i][0])
		return (print_sorted_env(data), 0);
	while (args[i])
	{
		if (!((args[i][0] >= 'A' && args[i][0] <= 'Z') ||
			(args[i][0] >= 'a' && args[i][0] <= 'z') ||
			(args[i][0] == '_')))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n", args[i]);
			i++;
			flag = 1;
			continue ;
		}
		string_check(args, i, j, &flag, data);
		i++;
	}
	return (flag);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:50 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 02:36:04 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_cd_path(char **args, t_data *data)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value("HOME", data);
		if (!path)
		{
			ft_printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

static int	arg_count(char **args)
{
	int	i;

	i = 0;
	if (!args || !args[i])
		return (0);
	while (args[i])
		i++;
	return (i);
}

void	update_env(t_data *data, char *value)
{
	t_env *(tmp);
	char *(cwd);
	tmp = data->env;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	if (!tmp)
		return (add_env_node(&data->env, new_env_node(ft_strdup(value), \
		ft_strdup(cwd))), free(cwd), (void)0);
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, value))
		{
			tmp->value = ft_strdup(cwd);
			break ;
		}
		else if (!tmp->next)
		{
			add_env_node(&data->env, new_env_node(ft_strdup(value), \
			ft_strdup(cwd)));
			break ;
		}
		tmp = tmp->next;
	}
	free(cwd);
}

int	builtin_cd(char **args, t_data *data)
{
	char *(path), *(old_pwd), *(new_pwd);
	if (arg_count(args) > 2)
		return (ft_printf("minishell: cd: too many arguments\n"), 1);
	old_pwd = getcwd(NULL, 0);
	path = get_cd_path(args, data);
	if (!path)
	{
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	if (old_pwd)
		(update_env(data, "OLDPWD"), free(old_pwd));
	if (path[0] && chdir(path) == -1)
	{
		ft_printf("minishell: cd: %s: %s\n", path, strerror(errno));
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
		(update_env(data, "PWD"), free(new_pwd));
	return (0);
}

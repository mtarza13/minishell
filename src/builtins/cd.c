/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:50 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/30 04:41:57 by yabarhda         ###   ########.fr       */
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
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

static int arg_count(char **args)
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
	char	*cwd;
	
	tmp = data->env;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	if (!tmp)
	{
		add_env_node(&data->env, new_env_node(ft_strdup(value), ft_strdup(cwd)));
		free(cwd);
		return ;
	}
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, value))
		{
			tmp->value = ft_strdup(cwd);
			break ;
		}
		else if (!tmp->next)
		{
			add_env_node(&data->env, new_env_node(ft_strdup(value), ft_strdup(cwd)));
			break ;
		}
		tmp = tmp->next;
	}
	free(cwd);
}

int	builtin_cd(char **args, t_data *data)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (arg_count(args) > 2)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);	
	old_pwd = getcwd(NULL, 0);
	path = get_cd_path(args, data);
	if (!path)
	{
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	if (old_pwd)
	{
		update_env(data, "OLDPWD");
		// set_env_value(env, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		update_env(data, "PWD");
		// set_env_value(env, "PWD", new_pwd);
		free(new_pwd);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:00:50 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/01 14:48:51 by mtarza           ###   ########.fr       */
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

/* static void	getcwd_perror(void)
{
	char	*cd;
	char	*getcwd;
	char	*no_file;

	cd = ft_strdup("cd: error retrieving current directory: ");
	getcwd = ft_strdup("getcwd: cannot access parent directories: ");
	no_file = ft_strdup("No such file or directory");
	ft_printf("%s\n", ft_strjoin(cd, ft_strjoin(getcwd, no_file)));
} */

int	builtin_cd(char **args, t_data *data)
{
	char *(path), *(old_pwd), *(new_pwd);
	if (arg_count(args) > 2)
		return (ft_printf("minishell: cd: too many arguments\n"), 1);
	old_pwd = getcwd(NULL, 0);
	// if (!old_pwd && args[1] && access(args[1], F_OK) == -1)
	// 	return (getcwd_perror(), 1);
	path = get_cd_path(args, data);
	if (!path)
	{
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
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

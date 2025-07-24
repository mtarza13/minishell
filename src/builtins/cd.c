
#include "../../include/minishell.h"

static char	*get_cd_path(char **args, t_env *env)
{
	char	*path;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		path = get_env_value(env, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(env, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		printf("%s\n", path);
	}
	else
		path = args[1];
	return (path);
}

int	builtin_cd(char **args, t_env *env)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	path = get_cd_path(args, env);
	if (!path)
	{
		if (old_pwd)
			free(old_pwd);
		return (1);
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
	if (old_pwd)
	{
		set_env_value(env, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		set_env_value(env, "PWD", new_pwd);
		free(new_pwd);
	}
	return (0);
}
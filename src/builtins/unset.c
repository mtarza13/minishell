
#include "../../include/minishell.h"

int	builtin_unset(char **args, t_env *env)
{
	int	exit_status;
	int	i;

	if (!args[1])
	{
		ft_putstr_fd("minishell: unset: not enough arguments\n", 2);
		return (1);
	}
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			exit_status = 1;
		}
		else
			unset_env_value(env, args[i]);
		i++;
	}
	return (exit_status);
}
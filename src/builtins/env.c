
#include "../../include/minishell.h"

int	builtin_env(char **args, t_env *env)
{
	int	i;

	(void)args;
	i = 0;
	while (i < env->count)
	{
		if (env->envp[i] && ft_strchr(env->envp[i], '='))
			printf("%s\n", env->envp[i]);
		i++;
	}
	return (0);
}
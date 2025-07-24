
#include "../../include/minishell.h"

int	builtin_exit(char **args, t_env *env)
{
	int	code;

	(void)env;
	printf("exit\n");
	if (args[1])
	{
		code = ft_atoi(args[1]);
		exit(code);
	}
	exit(0);
}
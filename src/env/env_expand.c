
#include "../../include/minishell.h"

char	**env_to_array(t_env *env)
{
	char	**array;
	int		i;
	int		j;

	if (!env)
		return (NULL);
	array = malloc(sizeof(char*) * (env->count + 1));
	if (!array)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < env->count)
	{
		if (env->envp[i] && ft_strchr(env->envp[i], '='))
			array[j++] = ft_strdup(env->envp[i]);
	}
	array[j] = NULL;
	return (array);
}
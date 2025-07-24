
#include "../../include/minishell.h"

static int	count_env(char **envp)
{
	int	count;

	count = 0;
	if (!envp)
		return (0);
	while (envp[count])
		count++;
	return (count);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		count;
	int		i;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	count = count_env(envp);
	env->capacity = count + 50;
	env->envp = malloc(sizeof(char *) * env->capacity);
	if (!env->envp)
	{
		free(env);
		return (NULL);
	}
	i = -1;
	while (++i < count)
		env->envp[i] = ft_strdup(envp[i]);
	while (i < env->capacity)
		env->envp[i++] = NULL;
	env->count = count;
	env->exit_status = 0;
	return (env);
}

void	free_env(t_env *env)
{
	if (!env)
		return ;
	ft_free_array(env->envp);
	free(env);
}
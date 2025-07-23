#include "../include/all.h"


static int count_env(char **envp)
{
	int count;
	count = 0;
	if(!*envp)
		return 0;
	while(envp[count])
		count++;
	return count;
}

void free_env(t_env *env, int i)
{
	while(--i < 0)
		free(env->envp[i]);
	free(env->envp);
	free(env);
}

t_env *init_env(char **envp)
{
	int i;
	t_env *env;
 	i = 0;

	env = malloc(sizeof(t_env));
	if(!env)
		return (NULL);

	env->count = count_env(envp);
	env->capacity = env->count + 50;

	env->envp = malloc(sizeof(char *) * env->capacity);
	if(!env->envp)
		return (free(env),NULL);

	while(i < env->count)
	{
		env->envp[i] = strdup(envp[i]);
		if(!env->envp)
			free_env(env,i);
		i++;
	}
	env->envp[i] = NULL;
	i = env->count;
	while(i < env->capacity)
	{
		env->envp[i] = NULL;
		i++;
	}
	env->count = i;
	env->exit_status = 0;
	return env;
}

#include "mini.h"


int count_env(char **envp)
{
	int i;
	i = 0;
	if(!envp)
		return (0);
	while(envp[i])
		i++;
	return i;
}

t_env *init_env(char **envp) {
    t_env *env;
	env = malloc(sizeof(t_env));
    if (!env) 
		return (NULL);
    int count;
	count = count_env(envp);
    env->capacity = count + 50;
    env->envp = malloc(sizeof(char *) * env->capacity);
    if (!env->envp){
		free(env); return NULL; 
	}
	int i ;
	i = 0;
	while(i < count)
	{
		env->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	i = count;
	while(i < env->capacity)
	{
		env->envp[i] = NULL;
		i++;
	}
	env->envp[i] = NULL;
    env->count = count;
	env->exit_status = 0;
    return env;
}

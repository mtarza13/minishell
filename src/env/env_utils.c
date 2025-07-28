#include "../../include/minishell.h"

char	*get_env_value(char *var, t_env *env)
{
	int (i), v_len = ft_strlen(var);
	t_env *(tmp);
	i = 0;
	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, var, v_len))
			return (tmp->value);
		tmp = tmp->next;
		i++;
	}
	return (NULL);
}

// int set_env_value(t_env *env, char *name, char *value)
// {
//     int i;
//     int len;
//     char *new_var;

//     len = ft_strlen(name);
//     i = 0;
//     while (env->envp[i])
//     {
//         if (ft_strncmp(env->envp[i], name, len) == 0 && env->envp[i][len] == '=')
//         {
//             free(env->envp[i]);
//             new_var = ft_strjoin(name, "=");
//             env->envp[i] = ft_strjoin(new_var, value);
//             free(new_var);
//             return (1);
//         }
//         i++;
//     }
 
//     new_var = ft_strjoin(name, "=");
//     env->envp[env->count] = ft_strjoin(new_var, value);
//     free(new_var);
//     env->count++;
//     env->envp[env->count] = NULL; 
//     return (1);
// }

int unset_env_value(t_env *env, char *name)
{
    int i;
    int j;
    int len;

    len = ft_strlen(name);
    i = 0;
    while (env->envp[i])
    {
        if (ft_strncmp(env->envp[i], name, len) == 0 && env->envp[i][len] == '=')
        {
            free(env->envp[i]);
            j = i;
            while (env->envp[j])
            {
                env->envp[j] = env->envp[j + 1];
                j++;
            }
            env->count--;
            return (1);
        }
        i++;
    }
    return (0);
}

char **env_to_array(t_env *env)
{
    char **array;
    int i;

    if (!env || !env->envp)
        return (NULL);
    array = malloc(sizeof(char *) * (env->count + 1));
    if (!array)
        return (NULL);
    i = 0;
    while (i < env->count)
    {
        array[i] = ft_strdup(env->envp[i]);
        i++;
    }
    array[i] = NULL;
    return (array);
}

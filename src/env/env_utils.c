#include "../../include/minishell.h"

char	*get_env_value(char *var, t_data *data)
{
	int (i), v_len = ft_strlen(var);
	i = 0;
	while (data->env)
	{
		if (!ft_strncmp(data->env->key, var, v_len))
			return (data->env->value);
		data->env = data->env->next;
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

// int unset_env_value(t_env *env, char *name)
// {
//     int i;
//     int j;
//     int len;

//     len = ft_strlen(name);
//     i = 0;
//     while (env->envp[i])
//     {
//         if (ft_strncmp(env->envp[i], name, len) == 0 && env->envp[i][len] == '=')
//         {
//             free(env->envp[i]);
//             j = i;
//             while (env->envp[j])
//             {
//                 env->envp[j] = env->envp[j + 1];
//                 j++;
//             }
//             env->count--;
//             return (1);
//         }
//         i++;
//     }
//     return (0);
// }

static int env_count(t_env *env)
{
	int i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

char **env_to_array(t_data *data)
{
    char **array;
    int i;

    if (!data->env || !data->envp)
        return (NULL);
    array = ft_malloc(sizeof(char *) * (env_count(data->env) + 1), 69);
    i = 0;
    while (i < env_count(data->env))
    {
        array[i] = ft_strdup(data->envp[i]);
        i++;
    }
    array[i] = NULL;
    return (array);
}

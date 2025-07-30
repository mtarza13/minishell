/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:25:13 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/30 04:38:24 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(char *var, t_data *data)
{
	int (i), v_len = ft_strlen(var);
	i = 0;
	t_env *tmp = data->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, var, v_len))
			return (tmp->value);
		tmp = tmp->next;
		i++;
	}
	return (NULL);
}

int	list_len(t_env *env)
{
	int		i;
	t_env	*tmp;

	i = 1;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

static int env_count(t_env *env)
{
	int i = 0;
	t_env *tmp;
	tmp = env;
	while (tmp)
	{
		tmp = tmp->next;
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

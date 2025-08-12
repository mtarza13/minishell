/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expantion_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 00:05:24 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/12 20:21:17 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*add_char(char *result, char c)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	return (ft_strjoin(result, temp));
}

char	*get_env(t_data *data, char *value)
{
	t_env	*env;

	env = data->env;
	while (env)
	{
		if (ft_strcmp(env->key, value) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

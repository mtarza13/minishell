/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:05:36 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 01:07:01 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_env_node(t_env *env, char *key, char *value)
{
	int (v_len);
	v_len = ft_strlen(key);
	while (env)
	{
		if (!ft_strncmp(env->key, key, v_len))
		{
			env->value = value;
			return ;
		}
		env = env->next;
	}
}

int	is_env_char(char c)
{
	return ((c >= 'a' && c <= 'z') || \
			(c >= 'A' && c <= 'Z') || \
			(c >= '0' && c <= '9') || \
			c == '_');
}

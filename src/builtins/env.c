/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:32:17 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 20:52:24 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(char **args, t_data *data)
{
	(void)args;

	while (data->env)
	{
		printf("%s=%s\n", data->env->key, data->env->value);
		data->env = data->env->next;
	}
	return (0);
}
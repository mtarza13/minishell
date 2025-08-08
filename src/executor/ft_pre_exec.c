/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pre_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 10:08:28 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/08 04:52:32 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_status(int status)
{
	if ((WTERMSIG(status) + 128) == 131 || (WTERMSIG(status) + 128) == 130)
		ft_printf("\n");
}

int	execute(t_cmd *cmd, t_data *data)
{
	if (data->cc == 1)
		return (execute_single(cmd, data));
	else
		return (execute_multi(cmd, data));
}

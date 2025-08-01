/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:32:27 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/01 14:29:43 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_pwd(char **args, t_data *data)
{
	(void)args;
	ft_putstr_fd(ft_strjoin(get_env_value("PWD", data), "\n"), 1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:01:54 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 21:25:14 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_flag(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	if (s[i] != '-')
		return (1);
	while (s[++i])
	{
		if (s[i] != 'n')
			return (1);
	}
	return (0);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = is_valid_flag(args[i]);
	if (!newline)
		while (!is_valid_flag(args[++i]))
			;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}

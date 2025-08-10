/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:40:34 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/10 21:14:58 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	p_syntax_err(char *s)
{
	ft_printf("minishell: syntax error near unexpected token `%s'\n", s);
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_operater_char(char in)
{
	if (in == '|' || in == '>' || in == '<')
		return (1);
	return (0);
}

void	if_nocmd(t_cmd *cmd)
{
	if (!cmd->args[0])
		free_n_exit(0);
}

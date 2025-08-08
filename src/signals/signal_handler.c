/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:29:08 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/08 15:18:23 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	sigint_handle(int sig)
{
	(void)sig;
	g_signal_received = 1;
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	heredoc_child_handle(int sig)
{
	(void)sig;
	ft_malloc(0, 0);
	exit(130);
}

void	heredoc_sigint_handle(int sig)
{
	(void)sig;
	g_signal_received = 1;
}

void	signals_execute_child(int sig)
{
	ft_malloc(0, 0);
	exit(sig + 128);
}

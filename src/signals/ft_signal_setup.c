/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal_setup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:28:54 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/12 20:17:05 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setup_signals(void)
{
	signal(SIGINT, sigint_handle);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_heredoc(void)
{
	signal(SIGINT, heredoc_sigint_handle);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_heredoc_child(void)
{
	signal(SIGINT, heredoc_child_handle);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_execute(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

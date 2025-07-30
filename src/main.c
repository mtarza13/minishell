/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:58:43 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 00:21:47 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal_received = 0;

void	free_data(void)
{
	ft_malloc(0, 0);
	rl_clear_history();
}

void	init_data(t_data *data, char **envp)
{
	rl_outstream = stderr;
	data->status = 0;
	data->env = init_env(envp);
	data->envp = ft_envp(data->env);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		exit_status;

	(void)argc;
	(void)argv;
	data = ft_malloc(sizeof(t_data), 1337);
	init_data(data, envp);
	minishell(data);
	exit_status = data->status;
	free_data();
	// ft_malloc(0, 0);
	return (exit_status);
}

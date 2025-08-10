/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 04:54:04 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/10 16:12:22 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_pipes(t_data *data)
{
	int (i);
	i = -1;
	while (++i < (data->cc - 1))
	{
		close(data->pipe[i][0]);
		close(data->pipe[i][1]);
	}
}

void	open_pipes(t_cmd *cmd)
{
	int (i);
	i = 0;
	cmd->data->pipe = (int **)ft_malloc(sizeof(int *) * (cmd->data->cc - 1), 1);
	while (i < (cmd->data->cc - 1))
	{
		cmd->data->pipe[i] = (int *)ft_malloc(sizeof(int) * 2, 1337);
		if (pipe(cmd->data->pipe[i]) == -1)
			free_n_exit(1);
		i++;
	}
}

int	wait_childs(t_data *data, int *pid)
{
	int *(status), i = -1, f = 0, idx = data->cc - 1;
	close_pipes(data);
	status = ft_malloc(sizeof(int) * data->cc, 1337);
	while (++i < data->cc && waitpid(pid[i], &status[i], 0) > 0)
		;
	i = -1;
	while (++i < data->cc)
	{
		if (WIFSIGNALED(status[i]))
		{
			f = 1;
			break ;
		}
	}
	if (WIFEXITED(status[idx]))
		return (exit_status(status[idx], f), WEXITSTATUS(status[idx]));
	if (WIFSIGNALED(status[idx]))
		return (exit_status(status[idx], f), 128 + WTERMSIG(status[idx]));
	return (0);
}

void	free_n_exit(int status)
{
	ft_malloc(0, 0);
	exit(status);
}

void	if_redirs(t_cmd *cmd)
{
	if (cmd->in != 0)
		(dup2(cmd->in, 0), close(cmd->in));
	if (cmd->out != 1)
		(dup2(cmd->out, 1), close(cmd->out));
}

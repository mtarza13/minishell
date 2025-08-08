/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 04:54:04 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/08 16:37:45 by yabarhda         ###   ########.fr       */
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
		cmd->data->pipe[i] = (int *)ft_malloc(sizeof(int) * 2, 2);
		if (pipe(cmd->data->pipe[i]) == -1)
			free_n_exit(1);
		i++;
	}
}

int	wait_childs(t_data *data, int *pid)
{
	int	i;
	int	status;

	i = -1;
	close_pipes(data);
	while (++i < data->cc && waitpid(pid[i], &status, 0) > 0)
		;
	if (WIFEXITED(status))
		return (exit_status(status), WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (exit_status(status), 128 + WTERMSIG(status));
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

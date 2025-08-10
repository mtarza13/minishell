/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 03:42:22 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/10 21:16:19 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_multi(t_cmd *cmd, t_data *data)
{
	char *(file);
	int (stat), *(pid), i = -1;
	pid = ft_malloc(sizeof(int) * data->cc, 1337);
	open_pipes(cmd);
	while (++i < data->cc)
	{
		pid[i] = fork();
		if (!pid[i])
		{
			signals_child();
			if (redirs(cmd, i))
				free_n_exit(1);
			(if_redirs(cmd), close_pipes(data), if_nocmd(cmd));
			if (is_builtin(cmd->args[0]))
			{
				stat = execute_builtin(cmd->args, data);
				(ft_malloc(0, 0), exit(stat));
			}
			file = filename(cmd->args[0], data);
			execve(file, cmd->args, data->envp);
			free_n_exit(1);
		}
		cmd = cmd->next;
	}
	return (wait_childs(data, pid));
}

int	exec_builtin(t_cmd *cmd)
{
	int	new_out;
	int	status;
	int	f;

	status = redirs(cmd, -1);
	f = cmd->out != STDOUT_FILENO;
	if (f)
	{
		new_out = dup(STDOUT_FILENO);
		(dup2(cmd->out, STDOUT_FILENO), close(cmd->out));
	}
	if (!status)
		status = execute_builtin(cmd->args, cmd->data);
	if (f)
		(dup2(new_out, STDOUT_FILENO), close(new_out));
	return (status);
}

int	execute_single(t_cmd *cmd, t_data *data)
{
	int (pid), status;
	char *(file);
	if (is_builtin(cmd->args[0]))
		return (exec_builtin(cmd));
	else
	{
		pid = fork();
		if (!pid)
		{
			signals_child();
			if (redirs(cmd, -1))
				free_n_exit(1);
			(if_redirs(cmd), if_nocmd(cmd));
			file = filename(cmd->args[0], data);
			execve(file, cmd->args, data->envp);
			free_n_exit(1);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (exit_status(status, 0), WEXITSTATUS(status));
		if (WIFSIGNALED(status))
			return (exit_status(status, 0), 128 + WTERMSIG(status));
	}
	return (0);
}

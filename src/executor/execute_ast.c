/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:33:07 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/29 21:55:02 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	execute_pipe(t_ast *ast, t_data *data)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;
	int		ex_status;

	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		ex_status = exec_ast(ast->left, data);
		ft_malloc(0, 0);
		exit(ex_status);
	}
	else if (pid1 > 0)
	{
		pid2 = fork();
		if (pid2 == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			ex_status = exec_ast(ast->left, data);
			ft_malloc(0, 0);
			exit(ex_status);
		}
		else if (pid2 > 0)
		{
			close(fd[0]);
			close(fd[1]);
			waitpid(pid1, &status1, 0);
			waitpid(pid2, &status2, 0);
			data->status = WIFEXITED(status2) ? WEXITSTATUS(status2) : 1;
			return (data->status);
		}
	}
	return (1);
}

int	exec_ast(t_ast *ast, t_data *data)
{
	char	**expanded;
	int		status;

	if (!ast)
		return (1);
	if (ast->type == NODE_COMMAND)
	{
		expanded = expand_args_professional(ast->args, data);
		status = execute_command(expanded, data, ast->redirs);
		ft_free_array(expanded);
		return (status);
	}
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, data));
	return (1);
}

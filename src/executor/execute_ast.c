/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:33:07 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/01 05:36:43 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_left(t_ast *ast, t_data *data, int *fd)
{
	int	exit_status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	exit_status = exec_ast(ast->right, data);
	(ft_malloc(0, 0), exit(exit_status));
}

void	exec_right(t_ast *ast, t_data *data, int *fd)
{
	int	exit_status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	exit_status = exec_ast(ast->left, data);
	(ft_malloc(0, 0), exit(exit_status));
}

static int	execute_pipe(t_ast *ast, t_data *data)
{
	int		fd[2];
	pid_t	pid[2];
	int		status;

	pipe(fd);
	pid[0] = fork();
	if (!pid[0])
		exec_right(ast, data, fd);
	else
	{
		pid[1] = fork();
		if (!pid[1])
			exec_left(ast, data, fd);
		else
		{
			(close(fd[0]), close(fd[1]));
			(waitpid(pid[0], &status, 0), waitpid(pid[1], &status, 0));
			if (WIFEXITED(status))
				data->status = WEXITSTATUS(status);
			else
				data->status = 1;
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
		expanded = expand_arg_array(ast->args, data);
		status = execute_command(expanded, data, ast->redirs);
		return (status);
	}
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, data));
	return (1);
}

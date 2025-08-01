/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 10:08:28 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 04:13:53 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	execute_external_command(char **args, t_data *data, t_redir *redirs)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*file;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (!setup_redirections(redirs, data))
			(ft_malloc(0, 0), exit(EXIT_FAILURE));
		envp = env_to_array(data);
		file = filename(args[0], data);
		execve(file, args, envp);
		ft_printf("minishell: %s: command not found\n", args[0]);
		(ft_malloc(0, 0), exit(COMMAND_NOT_FOUND));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (ft_printf("\n"), 128 + WTERMSIG(status));
	return (1);
}

static int	execute_builtin_with_redirections(char **args, t_data *data,
		t_redir *redirs)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (!setup_redirections(redirs, data))
	{
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (1);
	}
	status = execute_builtin(args, data);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (status);
}

static int	redir_check(t_data *data, t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND)
		{
			if (!handle_output_redirection(current, data, 0))
				return (EXIT_FAILURE);
		}
		else if (current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_HEREDOC)
		{
			if (!handle_input_redirection(current, data, 1))
				return (EXIT_FAILURE);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	execute_command(char **args, t_data *data, t_redir *redirs)
{
	if (!args || !args[0])
	{
		if (redirs)
		{
			if (redir_check(data, redirs))
				return (EXIT_FAILURE);
			return (EXIT_SUCCESS);
		}
		return (EXIT_SUCCESS);
	}
	if (is_builtin(args[0]))
		return (execute_builtin_with_redirections(args, data, redirs));
	else
		return (execute_external_command(args, data, redirs));
}

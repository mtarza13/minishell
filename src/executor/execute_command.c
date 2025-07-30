/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 10:08:28 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/30 23:57:58 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	execute_external_command(char **args, t_data *data, t_redir *redirs)
{
	pid_t	pid;
	int		status;
	char	**envp;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (!setup_redirections(redirs, data))
		{
			ft_malloc(0, 0);
			exit(1);
		}
		envp = env_to_array(data);
		char *file = filename(args[0], data);
		execve(file, args, envp);
		ft_printf("minishell: %s: command not found\n", args[0]);
		ft_free_array(envp);
		ft_malloc(0, 0);
		exit(COMMAND_NOT_FOUND);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		return (128 + WTERMSIG(status));
	}
	return (1);
}

static int	execute_builtin_with_redirections(char **args, t_data *data,
			t_redir *redirs)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;
	// if(write(1,"",0) <= 0)
	// 	return 1;
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

int	execute_command(char **args, t_data *data, t_redir *redirs)
{
	if (!args || !args[0])
	{
		if (redirs)
		{
			t_redir *current = redirs;
			while (current)
			{
				if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND)
				{
					char **expanded;
					int fd;
					int flags;
					int word_count = 0;

					expanded = expand_args_professional(&current->target, data);
					if (!expanded) {
						ft_printf("minishell: ambiguous redirect\n");
						return (EXIT_FAILURE);
					}
					while(expanded[word_count]) word_count++;
					if (word_count != 1) {
						ft_free_array(expanded);
						ft_printf("minishell: ambiguous redirect\n");
						return (EXIT_FAILURE);
					}

					if (current->type == TOKEN_REDIR_APPEND)
						flags = O_WRONLY | O_CREAT | O_APPEND;
					else
						flags = O_WRONLY | O_CREAT | O_TRUNC;
					
					fd = open(expanded[0], flags, 0644);
					if (fd == -1) {
						perror(expanded[0]);
						ft_free_array(expanded);
						return (EXIT_FAILURE);
					}
					close(fd);
					ft_free_array(expanded);
				}
				else if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
				{
					char **expanded;
					int fd;
					int word_count = 0;

					expanded = expand_args_professional(&current->target, data);
					if (!expanded) {
						ft_printf("minishell: ambiguous redirect\n");
						return (EXIT_FAILURE);
					}
					while(expanded[word_count]) word_count++;
					if (word_count != 1) {
						ft_free_array(expanded);
						ft_printf("minishell: ambiguous redirect\n");
						return (EXIT_FAILURE);
					}
					fd = open(expanded[0], O_RDONLY);
					if (fd == -1)
					{
						ft_printf("minishell: %s: No such file or directory\n", expanded[0]);
						ft_free_array(expanded);
						return (EXIT_FAILURE);
					}
					close(fd);
					ft_free_array(expanded);
				}
				current = current->next;
			}
			return (EXIT_SUCCESS);
		}
		return (EXIT_SUCCESS);
	}
	if (is_builtin(args[0]))
		return (execute_builtin_with_redirections(args, data, redirs));
	else
		return (execute_external_command(args, data, redirs));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:24:24 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 16:07:19 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int analyze_heredoc_delimiter(char *delimiter, char **final_delimiter, int *should_expand, t_env *env)
// {
//     (void)env;
//     if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
//         *should_expand = 0;
//     else
//         *should_expand = 1;
//     *final_delimiter = remove_quotes_advanced(delimiter);
//     if (!*final_delimiter)
//         return (0);
//     return (1);
// }

// int single_heredoc_no_command(t_redir *redir, t_env *env)
// {
//     char *line;
//     char *final_delim;
//     int should_expand;

//     if (!analyze_heredoc_delimiter(redir->target, &final_delim, &should_expand,env))
// 		return (0);
// 	signal(SIGINT, handle_sigint_heredoc);
// 	signal(SIGQUIT, SIG_IGN);
// 	int pid = fork();
// 	if (!pid)
// 	{
//     	signal(SIGINT, handle_heredoc_signal);
// 		signal(SIGQUIT, SIG_IGN);
// 		while (1)
// 		{
// 			line = readline("> ");
// 			if (!line)
// 			{
// 				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
// 				ft_putstr_fd(final_delim, 2);
// 				ft_putstr_fd("')\n", 2);
// 				break;
// 			}
// 			if (ft_strcmp(line, final_delim) == 0)
// 			{
// 				free(line);
// 				break;
// 			}
// 			free(line);
// 		}
// 		free(final_delim);
// 		exit(0);
// 	}
//     free(final_delim);
// 	int status;
// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 	{
// 		if (WEXITSTATUS(status) == 130)
// 			return (-1);
// 		return (0);
// 	}
// 	if (WIFSIGNALED(status))
// 	{
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 		return (-1);
// 	}
//     return (0);
// }

char	*get_heredoc_filename(void)
{
	int fd = open("/dev/random", O_RDONLY);
	char	*filename = malloc(11);
	read(fd, filename, 10);
	filename[0] = '.';
	filename[10] = '\0';
	close(fd);
	return (filename);
}

void	heredoc_handle(char *file, char *dlimit, int expand, t_env *env)
{
	char *input;
	int fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			close(fd);
			exit(131);
		}
		if (!ft_strcmp(input, dlimit))
			break ;
		if (!expand)
			input = expand_variables_advanced(input, env);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
	}
	close(fd);
	exit(0);
}

int	heredoc_sig_status(char *dlimit, int status)
{
	if (WIFSIGNALED(status))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		return (0);
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
			return (0);
		else if (WEXITSTATUS(status) == 131)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(dlimit, 2);
			ft_putstr_fd("')\n", 2);
			return (1);
		}
	}	
	return (1);
}

int	feed_heredoc(char *file, char *dlimit, int expand, t_env *env)
{
	if (expand)
		dlimit = remove_quotes_advanced(dlimit);
	signals_heredoc();
	int pid, status;
	pid = fork();
	if (!pid)
	{
		signals_heredoc_child();
		heredoc_handle(file, dlimit, expand, env);
	}
	waitpid(pid, &status, 0);
	if (!heredoc_sig_status(dlimit, status))
		return (0);
	return (1);
}

int	heredoc_check(t_token *token, t_data *data)
{
	while (token)
	{
		if (token->type == TOKEN_HEREDOC)
		{
			char *dlimit = token->next->value;
			token->next->value = get_heredoc_filename();
			if (!feed_heredoc(token->next->value, dlimit, (ft_strchr(dlimit, '\'') || ft_strchr(dlimit, '"')), env))
				return (0);
		}
		token = token->next;
	}
	return (1);
}

// int	heredoc_check_single(t_redir *current, t_env *env)
// {
// 	while (current)
// 	{
// 		if (current->type == TOKEN_HEREDOC)
// 		{
// 			int	fd = single_heredoc_no_command(current, env);
// 			if (fd < 0)
// 				return (1);
// 		}
// 		current = current->next;
// 	}
// 	return (0);
// }

// int	heredoc_check_multi(t_redir *current, t_env *env)
// {
// 	while (current)
// 	{
// 		if (current->type == TOKEN_HEREDOC)
// 		{
// 			int	fd = setup_heredoc(current, env);
// 			if (fd < 0)
// 				return (1);
// 			close(fd); // Close the fd since we're just checking, not using
// 		}
// 		current = current->next;
// 	}
// 	return (0); // Changed from EXIT_SUCCESS to 0 for consistency
// }

// int setup_heredoc(t_redir *redir, t_env *env)
// {
//     char *line;
//     char *final_delim;
//     int should_expand;
//     int pipefd[2];

//     if (!analyze_heredoc_delimiter(redir->target, &final_delim, &should_expand,env))
//         return (-1);
//     if (pipe(pipefd) == -1)
//     {
//         free(final_delim);
//         perror("pipe");
//         return (-1);
//     }
//     signal(SIGINT, handle_heredoc_signal_multi);
//     while (1)
//     {
//         line = readline("> ");
//         if (!line) // EOF (Ctrl+D)
//         {
//             ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
//             ft_putstr_fd(final_delim, 2);
//             ft_putstr_fd("')\n", 2);
//             break;
//         }
//         if (ft_strcmp(line, final_delim) == 0)
//         {
//             free(line);
//             break;
//         }
        
//         char *line_to_write = line;
//         if (should_expand)
//             line_to_write = expand_variables_advanced(line, env);
//         write(pipefd[1], line_to_write, ft_strlen(line_to_write));
//         write(pipefd[1], "\n", 1);
//         if (should_expand && line_to_write != line)
//             free(line_to_write);
//         free(line);
//     }
    
//     free(final_delim);
//     close(pipefd[1]);
    
//     // Restore normal signal handling
//     setup_signals();
    
//     if (g_signal_received)
//     {
//         close(pipefd[0]);
//         g_signal_received = 0; // Reset for next command
//         return (-1);
//     }
    
//     return (pipefd[0]);
// }

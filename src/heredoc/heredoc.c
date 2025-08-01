/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:24:24 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 00:32:49 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_heredoc_filename(void)
{
	int		fd;
	char	*filename;

	fd = open("/dev/random", O_RDONLY);
	filename = ft_malloc(11, 69);
	read(fd, filename, 10);
	filename[0] = '.';
	filename[10] = '\0';
	close(fd);
	return (filename);
}

void	heredoc_handle(char *file, char *dlimit, int expand, t_data *data)
{
	int		fd;
	char	*input;

	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			close(fd);
			ft_malloc(0, 0);
			exit(131);
		}
		if (!ft_strcmp(input, dlimit))
			break ;
		if (expand)
			input = expand_arg_list(input, data);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
	}
	close(fd);
	ft_malloc(0, 0);
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
			ft_printf("minishell: warning: here-document delimited by \
				end-of-file (wanted `%s')", dlimit);
			return (1);
		}
	}	
	return (1);
}

int	feed_heredoc(char *file, char *dlimit, int expand, t_data *data)
{
	int	pid;
	int	status;

	if (expand)
		dlimit = remove_quotes_advanced(dlimit);
	signals_heredoc();
	pid = fork();
	if (!pid)
	{
		signals_heredoc_child();
		heredoc_handle(file, dlimit, expand, data);
	}
	waitpid(pid, &status, 0);
	if (!heredoc_sig_status(dlimit, status))
		return (0);
	return (1);
}

int	heredoc_check(t_token *token, t_data *data)
{
	char	*dlimit;

	while (token)
	{
		if (token->type == TOKEN_HEREDOC)
		{
			dlimit = token->next->value;
			token->next->value = get_heredoc_filename();
			if (!feed_heredoc(token->next->value, dlimit, \
				(ft_strchr(dlimit, '\'') || ft_strchr(dlimit, '"')), data))
				return (0);
		}
		token = token->next;
	}
	return (1);
}

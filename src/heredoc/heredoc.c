
#include "../../include/minishell.h"

static char	*process_heredoc_content(char *delimiter, int should_expand,
			t_env *env)
{
	char			*content;
	char			*line;
	char			*processed_line;
	int				content_len;

	content = malloc(MAX_HEREDOC_SIZE);
	if (!content)
		return (NULL);
	content[0] = '\0';
	content_len = 0;
	signal(SIGINT, handle_heredoc_signal);
	while ((line = readline("> ")))
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (should_expand)
		{
			processed_line = expand_variables_advanced(line, env);
			free(line);
		}
		else
			processed_line = line;
		if (!processed_line)
			processed_line = ft_strdup("");
		if (content_len + ft_strlen(processed_line) + 2 >= MAX_HEREDOC_SIZE)
		{
			ft_putstr_fd("minishell: heredoc too large\n", 2);
			free(processed_line);
			free(content);
			return (NULL);
		}
		ft_strlcat(content + content_len, processed_line,
			MAX_HEREDOC_SIZE - content_len);
		content_len += ft_strlen(processed_line);
		content[content_len++] = '\n';
		content[content_len] = '\0';
		free(processed_line);
	}
	signal(SIGINT, SIG_DFL);
	return (content);
}

int	setup_heredoc_complete(t_redir *redir, t_env *env)
{
	char	*final_delimiter;
	char	*content;
	char	*temp;
	int		should_expand_content;
	int		pipe_fds[2];
	pid_t	pid;

	if (!analyze_heredoc_delimiter(redir->target, &final_delimiter,
			&should_expand_content))
		return (-1);
	if (should_expand_content)
	{
		temp = expand_variables_advanced(redir->target, env);
		if (temp)
		{
			free(final_delimiter);
			final_delimiter = remove_quotes_advanced(temp);
			free(temp);
		}
	}
	content = process_heredoc_content(final_delimiter, should_expand_content,
			env);
	free(final_delimiter);
	if (!content)
		return (-1);
	if (pipe(pipe_fds) == -1)
	{
		free(content);
		return (-1);
	}
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fds[0]);
		write(pipe_fds[1], content, ft_strlen(content));
		close(pipe_fds[1]);
		free(content);
		exit(0);
	}
	else if (pid > 0)
	{
		close(pipe_fds[1]);
		free(content);
		waitpid(pid, NULL, 0);
		return (pipe_fds[0]);
	}
	free(content);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	return (-1);
}
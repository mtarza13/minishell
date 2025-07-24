
#include "../../include/minishell.h"

static int	handle_input_redirection(t_redir *redir, t_env *env)
{
	char	**expanded;
	int		fd;

	if (redir->type == TOKEN_HEREDOC)
	{
		fd = setup_heredoc_complete(redir, env);
		if (fd == -1)
			return (0);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			perror("minishell: dup2");
			return (0);
		}
		close(fd);
		return (1);
	}
	else if (redir->type == TOKEN_REDIR_IN)
	{
		expanded = expand_args_professional(&redir->target, env);
		if (!expanded || !expanded[0])
		{
			ft_free_array(expanded);
			ft_putstr_fd("minishell: ambiguous redirect\n", 2);
			return (0);
		}
		fd = open(expanded[0], O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(expanded[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
			ft_free_array(expanded);
			return (0);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			perror("minishell: dup2");
			ft_free_array(expanded);
			return (0);
		}
		close(fd);
		ft_free_array(expanded);
		return (1);
	}
	return (1);
}

static int	handle_output_redirection(t_redir *redir, t_env *env)
{
	char	**expanded;
	int		flags;
	int		fd;

	expanded = expand_args_professional(&redir->target, env);
	if (!expanded || !expanded[0])
	{
		ft_free_array(expanded);
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		return (0);
	}
	flags = O_WRONLY | O_CREAT;
	if (redir->type == TOKEN_REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(expanded[0], flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(expanded[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		ft_free_array(expanded);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		perror("minishell: dup2");
		ft_free_array(expanded);
		return (0);
	}
	close(fd);
	ft_free_array(expanded);
	return (1);
}

int	setup_redirections(t_redir *redirs, t_env *env)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
		{
			if (!handle_input_redirection(current, env))
				return (0);
		}
		else if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND)
		{
			if (!handle_output_redirection(current, env))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

#include "../../include/minishell.h"

t_redir	*create_redirection(t_token_type type, char *target)
{
	t_redir	*redir;
	int		should_expand;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = ft_strdup(target);
	redir->expanded_target = NULL;
	redir->quoted_delimiter = 0;
	redir->fd = -1;
	redir->next = NULL;
	if (type == TOKEN_HEREDOC)
	{
		if (analyze_heredoc_delimiter(target, &redir->expanded_target,
				&should_expand))
			redir->quoted_delimiter = !should_expand;
	}
	return (redir);
}

void	add_redirection(t_redir **redirs, t_redir *new_redir)
{
	t_redir	*current;

	if (!*redirs)
	{
		*redirs = new_redir;
		return ;
	}
	current = *redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

int	parse_single_redirection(t_token **tokens, t_redir **redirs)
{
	t_token_type	redir_type;
	t_redir			*new_redir;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (0);
	}
	new_redir = create_redirection(redir_type, (*tokens)->value);
	if (!new_redir)
		return (0);
	add_redirection(redirs, new_redir);
	*tokens = (*tokens)->next;
	return (1);
}
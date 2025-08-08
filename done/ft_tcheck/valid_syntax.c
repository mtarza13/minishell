
int	is_operator(t_type_token type)
{
	if (type == TOKEN_PIPE || type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC)
		return (1);
	return (0);
}

int	is_redir(t_type_token type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC)
		return (1);
	return (0);
}

int	valid_input(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	if (tmp == NULL)
		return (1);
	if (tmp->type == TOKEN_PIPE)
		(printf("syntax error near unexpected token `|'\n"), return (0));
	while (tmp)
	{
		if (tmp->type == TOKEN_PIPE)
		{
			if (tmp->next && tmp->next->type == TOKEN_PIPE)
				(printf("syntax error near unexpected token `|'\n"),
					return (0));
			if (tmp->next == NULL)
				(printf("syntax error near unexpected token `newline'\n"),
					return (0));
		}
		if (is_redir(tmp->type))
		{
			if (tmp->next == NULL || tmp->next->type != TOKEN_WORD)
			{
				if (tmp->next == NULL)
					(printf("syntax error near unexpected token `newline'\n"),
						return (0));
				else
					(printf("syntax error near unexpected token `%s'\n",
							tmp->next->value), return (0));
			}
		}
		if (is_redir(tmp->type) && tmp->next && is_operator(tmp->next->type)
			&& tmp->next->type != TOKEN_WORD)
			(printf("syntax error near unexpected token `%s'\n",
					tmp->next->value), return (0));
		tmp = tmp->next;
	}
	return (1);
}

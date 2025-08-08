#include ""

t_token	*tokinaiz_op(char *line, t_token *token, int *i)
{
	if (line[*i] == '|')
		(token = add_token(token, creat_token(strdup("|"), TOKEN_PIPE)),
			(*i)++);
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		token = add_token(token, creat_token(strdup(">>"), TOKEN_REDIR_APPEND));
		(*i) += 2;
	}
	else if (line[*i] == '>')
	{
		token = add_token(token, creat_token(strdup(">"), TOKEN_REDIR_OUT));
		(*i)++;
	}
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		token = add_token(token, creat_token(strdup("<<"),
					TOKEN_REDIR_HEREDOC));
		(*i) += 2;
	}
	else if (line[*i] == '<')
	{
		token = add_token(token, creat_token(strdup("<"), TOKEN_REDIR_IN));
		(*i)++;
	}
	return (token);
}

t_token	*tokinaiz_word(char *line, t_token *token, int *i)
{
	int(start), (d_quote), (s_quote);
	start = *i;
	d_quote = 0;
	s_quote = 0;
	while (line[*i])
	{
		if (line[*i] == '\'')
			s_quote = !s_quote;
		if (line[*i] == '"')
			d_quote = !d_quote;
		if (s_quote == 0 && d_quote == 0 && (is_space(line[*i])
				|| is_operater_char(line[*i])))
			break ;
		(*i)++;
	}
	if (s_quote || d_quote)
	{
		printf("minishell: syntax error: unclosed quote\n");
		return (NULL);
	}
	if (*i > start)
		token = add_token(token, creat_token(strndup(line + start, *i - start),
					TOKEN_WORD));
	return (token);
}

t_token	*tokinaizer(char *line)
{
	t_token	*token;
	int		i;

	token = NULL;
	i = 0;
	if (line == NULL)
		return (NULL);
	while (line[i])
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (is_operater_char(line[i]))
			token = tokinaiz_op(line, token, &i);
		else
			token = tokinaiz_word(line, token, &i);
	}
	return (token);
}

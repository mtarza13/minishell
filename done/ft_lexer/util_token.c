
int	is_space(char s)
{
	if (s == ' ' || s == '\t' || s == '\n')
		return (1);
	return (0);
}

int	is_operater_char(char in)
{
	if (in == '|' || in == '>' || in == '<')
		return (1);
	return (0);
}

t_token	*creat_token(char *line, t_type_token type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->type = type;
	token->value = line;
	token->next = NULL;
	return (token);
}

t_token	*add_token(t_token *token, t_token *new)
{
	t_token	*t;

	if (token == NULL)
	{
		token = new;
		return (new);
	}
	t = token;
	while (t->next)
		t = t->next;
	t->next = new;
	return (token);
}

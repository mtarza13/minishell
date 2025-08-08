s_cmd	*creat_cmd(t_token **token)
{
	s_cmd	*cmd;

	cmd = malloc(sizeof(s_cmd));
	cmd->arg = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_redir	*creat_redir(t_type_token type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	redir->file = file;
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

t_redir	*add_redir(t_redir *head_red, t_redir *cmd_red)
{
	t_redir	*tmp;

	if (head_red == NULL)
		return (cmd_red);
	tmp = head_red;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd_red;
	return (head_red);
}

t_arg	*add_arg(t_arg *head_arg, char *new_cmd)
{
	t_arg	*node;
	t_arg	*tmp;

	node = malloc(sizeof(t_arg));
	if (new_cmd == NULL)
		return (head_arg);
	node->value = new_cmd;
	node->next = NULL;
	if (head_arg == NULL)
		return (node);
	tmp = head_arg;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return (head_arg);
}

s_cmd	*parse_cmd(t_token **token)
{
	s_cmd	*cmd;

	cmd = creat_cmd(token);
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (is_redir((*token)->type))
		{
			*token = (*token)->next;
			if (*token == NULL || (*token)->type != TOKEN_WORD)
				return (NULL);
			cmd->redir = add_redir(cmd->redir, creat_redir((*token)->type,
						(*token)->value));
			*token = (*token)->next;
		}
		else if ((*token)->type == TOKEN_WORD)
		{
			cmd->arg = add_arg(cmd->arg, (*token)->value);
			(*token) = (*token)->next;
		}
		else
			break ;
	}
	return (cmd);
}

s_cmd *parser(t_token *token)
{
    s_cmd *cmd;
    s_cmd *head = NULL;
    s_cmd *tmp;

    while (token)
    {
        cmd = parse_cmd(&token);
        if (cmd == NULL)
            return NULL;
        if (head == NULL)
        {
            head = cmd;
            tmp = head;
        }
        else
        {
            tmp->next = cmd;
            tmp = cmd;
        }
        if (token && token->type == TOKEN_PIPE)
            token = token->next;
    }
    return head;
}

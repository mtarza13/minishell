#include "../../include/minishell.h"

t_token *create_token(t_token_type type, char *value)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = NULL;
    if (value)
        token->value = ft_strdup(value);
    token->next = NULL;
    return (token);
}

void add_token(t_token **tokens, t_token *new_token)
{
    t_token *current;

    if (!*tokens)
    {
        *tokens = new_token;
        return;
    }
    current = *tokens;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
		if (tmp->type == TOKEN_HEREDOC)
			unlink(tmp->next->value);
        if (tmp->value && tmp->type != TOKEN_HEREDOC)
            free(tmp->value);
        free(tmp);
    }
}

int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

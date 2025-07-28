/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:53:48 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 21:53:57 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token *create_token(t_token_type type, char *value)
{
    t_token *token;

    token = ft_malloc(sizeof(t_token), 69);
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

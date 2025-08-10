/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:57:26 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/10 14:54:56 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*creat_token(char *line, t_type_token type)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token), 1337);
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

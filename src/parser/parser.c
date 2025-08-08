/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:57:57 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/08 16:57:01 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd *parser(t_token *token, t_data *data)
{
    t_cmd *cmd;
    t_cmd *head = NULL;
    t_cmd *tmp;

    while (token)
    {
        cmd = parse_cmd(&token, data);
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

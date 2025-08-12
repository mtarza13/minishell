/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:57:57 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/12 20:21:29 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_word_utils(t_token **token, t_data *data, t_cmd *cmd)
{
	char **(expand_args), *(tmp[2]);
	int (i);
	tmp[0] = (*token)->value;
	tmp[1] = NULL;
	i = 0;
	expand_args = NULL;
	if ((*token)->type == WORD)
	{
		expand_args = expand_arg_array(tmp, data);
		if (expand_args)
		{
			i = 0;
			while (expand_args[i])
				cmd->arg = add_arg(cmd->arg, expand_args[i++]);
		}
		(*token) = (*token)->next;
	}
	else
		return (0);
	return (1);
}

t_cmd	*parser(t_token *token, t_data *data)
{
	t_cmd	*cmd;
	t_cmd	*head;
	t_cmd	*tmp;

	head = NULL;
	while (token)
	{
		cmd = parse_cmd(&token, data);
		if (cmd == NULL)
			return (NULL);
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
		if (token && token->type == PIPE)
			token = token->next;
	}
	return (head);
}

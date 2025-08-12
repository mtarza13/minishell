/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_valid_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:56:59 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/12 20:21:44 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(t_type_token type)
{
	if (type == PIPE || type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == REDIR_HEREDOC)
		return (1);
	return (0);
}

int	is_redir(t_type_token type)
{
	if (type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
		|| type == REDIR_HEREDOC)
		return (1);
	return (0);
}

int	is_redir_check(t_token *tmp)
{
	if (is_redir(tmp->type))
	{
		if (tmp->next == NULL || tmp->next->type != WORD)
		{
			if (tmp->next == NULL)
				return (p_syntax_err("newline"), 0);
			else
				return (p_syntax_err(tmp->next->value), 0);
		}
		if (is_redir(tmp->type) && tmp->next && is_operator(tmp->next->type)
			&& tmp->next->type != WORD)
			return (p_syntax_err(tmp->next->value), 0);
	}
	return (1);
}

int	valid_input(t_token *token, t_data *data)
{
	t_token	*tmp;

	tmp = token;
	if (tmp == NULL)
		return (1);
	if (tmp->type == PIPE)
		return (p_syntax_err("|"), 0);
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			data->pipes = true;
			if (tmp->next && tmp->next->type == PIPE)
				return (p_syntax_err("|"), 0);
			if (tmp->next == NULL)
				return (p_syntax_err("newline"), 0);
		}
		if (!is_redir_check(tmp))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

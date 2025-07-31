/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:56:55 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 04:37:30 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	print_syntax_error(char *token_value, t_data *data)
{
	ft_printf("minishell: syntax error near unexpected token `%s'\n", \
		token_value);
	data->status = 2;
	return (0);
}

int	validate_syntax(t_token *tokens, t_data *data)
{
	t_token *(current);
	if (!tokens)
		return (1);
	current = tokens;
	if (current->type == TOKEN_PIPE)
		return (print_syntax_error(current->value, data));
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next)
				return (print_syntax_error("newline", data));
			if (current->next->type == TOKEN_PIPE)
				return (print_syntax_error(current->next->value, data));
		}
		if (is_redirection_token(current->type))
		{
			if (!current->next)
				return (print_syntax_error("newline", data));
			if (current->next->type != TOKEN_WORD)
				return (print_syntax_error(current->next->value, data));
		}
		current = current->next;
	}
	return (1);
}

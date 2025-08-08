/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:43:22 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/08 18:21:50 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	clean_up(t_token *token)
{
	while (token)
	{
		if (token->type == REDIR_HEREDOC)
			unlink(token->next->value);
		token = token->next;
	}
}

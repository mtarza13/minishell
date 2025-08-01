/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:27:30 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/01 04:32:58 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_command_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (is_redirection_token(current->type))
		{
			current = current->next;
			if (current)
				current = current->next;
		}
		else
		{
			count++;
			current = current->next;
		}
	}
	return (count);
}

static t_ast	*init_command_node(int argc)
{
	t_ast	*cmd;

	cmd = ft_malloc(sizeof(t_ast), 69);
	cmd->type = NODE_COMMAND;
	cmd->redirs = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->arg_count = argc;
	if (argc > 0)
		cmd->args = ft_malloc(sizeof(char *) * (argc + 1), 69);
	else
		cmd->args = NULL;
	return (cmd);
}

t_ast	*parse_command_with_redirections(t_token **tokens, t_data *data)
{
	t_ast *(cmd);
	int (argc), i = 0;
	argc = count_command_args(*tokens);
	cmd = init_command_node(argc);
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if (is_redirection_token((*tokens)->type))
		{
			if (!parse_single_redirection(tokens, &cmd->redirs, data))
				return (free_ast(cmd), NULL);
		}
		else if ((*tokens)->type == TOKEN_WORD)
		{
			if (cmd->args)
				cmd->args[i++] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	if (cmd->args)
		cmd->args[i] = NULL;
	if (argc == 0 && !cmd->redirs)
		return (free_ast(cmd), NULL);
	return (cmd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:27:37 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/05 14:53:20 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_malloc(sizeof(t_ast), 1337);
	node->type = NODE_PIPE;
	node->args = NULL;
	node->arg_count = 0;
	node->redirs = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

t_cmd	*create_cmd(t_data *data)
{
	t_cmd	*cmd;

	cmd = ft_malloc(sizeof(t_cmd), 3);
	cmd->args = NULL;
	cmd->in = 0;
	cmd->out = 1;
	cmd->redir = NULL;
	cmd->data = data;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!*head)
	{
		*head = new_cmd;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

t_ast	*parse_pipeline(t_token *token, t_data *data)
{
	t_cmd	*head;
	t_cmd	*node;
	t_token	*tmp;

	head = NULL;
	node = NULL;
	tmp = token;
	while (tmp)
	{
		
		if (tmp->type == TOKEN_PIPE)
		{
			data->pipes = true;
			
		}
		if (!token || (token)->type == TOKEN_PIPE)
		{
			ft_printf("minishell: syntax error near unexpected token `|'\n");
			data->status = 2;
			return (free_ast(left), NULL);
		}
		right = pars_node(token, data);
		if (!right)
			return (free_ast(left), NULL);
		left = create_pipe_node(left, right);
		tmp = tmp->next;
	}
	return (left);
}

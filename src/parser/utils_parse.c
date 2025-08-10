/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:57:14 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/10 22:07:32 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*creat_cmd(t_data *data)
{
	t_cmd	*cmd;

	cmd = ft_malloc(sizeof(t_cmd), 1337);
	cmd->data = data;
	cmd->arg = NULL;
	cmd->args = NULL;
	cmd->redir = NULL;
	cmd->in = -1;
	cmd->out = -1;
	cmd->next = NULL;
	return (cmd);
}

t_redir	*creat_redir(t_type_token type, char *file)
{
	t_redir	*redir;

	redir = ft_malloc(sizeof(t_redir), 1337);
	redir->file = file;
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

t_redir	*add_redir(t_redir *head_red, t_redir *cmd_red)
{
	t_redir	*tmp;

	if (head_red == NULL)
		return (cmd_red);
	tmp = head_red;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd_red;
	return (head_red);
}

t_arg	*add_arg(t_arg *head_arg, char *new_cmd)
{
	t_arg	*node;
	t_arg	*tmp;

	node = ft_malloc(sizeof(t_arg), 1337);
	if (new_cmd == NULL)
		return (head_arg);
	node->value = new_cmd;
	node->next = NULL;
	if (head_arg == NULL)
		return (node);
	tmp = head_arg;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return (head_arg);
}

t_cmd	*parse_cmd(t_token **token, t_data *data)
{
	t_cmd			*cmd;
	t_type_token	type;
	char			*expand_file;

	cmd = creat_cmd(data);
	while (*token && (*token)->type != PIPE)
	{
		if (is_redir((*token)->type))
		{
			type = (*token)->type;
			*token = (*token)->next;
			if (*token == NULL || (*token)->type != WORD)
				return (NULL);
			expand_file = remove_quote(expand_string((*token)->value, data));
			cmd->redir = add_redir(cmd->redir, creat_redir(type, expand_file));
			*token = (*token)->next;
		}
		else if (check_word_utils(token, data, cmd))
			continue ;
		else
			break ;
	}
	return (cmd);
}

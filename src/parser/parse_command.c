
#include "../../include/minishell.h"

static int	count_command_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			count++;
		}
		else if (is_redirection_token(current->type))
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current = current->next;
			continue ;
		}
		current = current->next;
	}
	return (count);
}

static int	parse_command_args(t_token **tokens, t_ast *cmd)
{
	int	arg_index;

	arg_index = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if (arg_index < cmd->arg_count)
				cmd->args[arg_index++] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
		else if (is_redirection_token((*tokens)->type))
		{
			if (!parse_single_redirection(tokens, &cmd->redirs))
				return (0);
		}
		else
			break ;
	}
	if (cmd->args)
		cmd->args[arg_index] = NULL;
	return (1);
}

t_ast	*parse_command_with_redirections(t_token **tokens)
{
	t_ast	*cmd;
	int		argc;

	if (!*tokens)
		return (NULL);
	cmd = malloc(sizeof(t_ast));
	if (!cmd)
		return (NULL);
	cmd->type = NODE_COMMAND;
	cmd->redirs = NULL;
	cmd->left = cmd->right = NULL;
	argc = count_command_args(*tokens);
	if (argc > 0)
	{
		cmd->args = malloc(sizeof(char*) * (argc + 1));
		if (!cmd->args)
		{
			free(cmd);
			return (NULL);
		}
	}
	else
		cmd->args = NULL;
	cmd->arg_count = argc;
	if (!parse_command_args(tokens, cmd))
	{
		free_ast(cmd);
		return (NULL);
	}
	if (argc == 0 && !cmd->redirs)
	{
		free_ast(cmd);
		return (NULL);
	}
	return (cmd);
}
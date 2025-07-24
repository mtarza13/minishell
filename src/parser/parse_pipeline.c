
#include "../../include/minishell.h"

static t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*pipe;

	pipe = malloc(sizeof(t_ast));
	if (!pipe)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
	pipe->type = NODE_PIPE;
	pipe->args = NULL;
	pipe->arg_count = 0;
	pipe->redirs = NULL;
	pipe->left = left;
	pipe->right = right;
	return (pipe);
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command_with_redirections(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
				2);
			free_ast(left);
			return (NULL);
		}
		right = parse_command_with_redirections(tokens);
		if (!right)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
			free_ast(left);
			return (NULL);
		}
		left = create_pipe_node(left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}
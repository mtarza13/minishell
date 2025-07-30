#include "../../include/minishell.h"

static int	print_syntax_error(char *token_value)
{
	ft_printf("minishell: syntax error near unexpected token `%s'\n", token_value);
	return (0);
}


int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1); 
	current = tokens;
	if (current->type == TOKEN_PIPE)
		return (print_syntax_error(current->value));
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next)
				return (print_syntax_error("newline"));
			if (current->next->type == TOKEN_PIPE)
				return (print_syntax_error(current->next->value));
		}
		if (is_redirection_token(current->type))
		{
			if (!current->next)
				return (print_syntax_error("newline"));
			if (current->next->type != TOKEN_WORD)
				return (print_syntax_error(current->next->value));
		}
		current = current->next;
	}
	return (1); 
}

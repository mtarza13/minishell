
#include "../../include/minishell.h"

int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	if (current->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
					2);
				return (0);
			}
			if (current->next->type == TOKEN_PIPE)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
					2);
				return (0);
			}
		}
		if (is_redirection_token(current->type))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token\n",
					2);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
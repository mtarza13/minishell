
#include "../../include/minishell.h"

void	free_redirections(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		tmp = redirs;
		redirs = redirs->next;
		if (tmp->target)
			free(tmp->target);
		if (tmp->expanded_target)
			free(tmp->expanded_target);
		free(tmp);
	}
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->args)
		ft_free_array(ast->args);
	free_redirections(ast->redirs);
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast);
}
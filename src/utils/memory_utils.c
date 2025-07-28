#include "../../include/minishell.h"

void free_redirections(t_redir *redirs)
{
    t_redir *tmp;

    while (redirs)
    {
        tmp = redirs;
        redirs = redirs->next;
		if (tmp->type == TOKEN_HEREDOC)
			unlink(tmp->target);
        free(tmp->target);
        if (tmp->expanded_target)
            free(tmp->expanded_target);
        free(tmp);
    }
}

void free_ast(t_ast *ast)
{
    if (!ast)
        return;
    if (ast->args)
        ft_free_array(ast->args);
    if (ast->redirs)
        free_redirections(ast->redirs);
    free_ast(ast->left);
    free_ast(ast->right);
    free(ast);
}

void ft_free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void free_env(t_env *env)
{
    if (!env)
        return;
    if (env->envp)
        ft_free_array(env->envp);
    free(env);
}

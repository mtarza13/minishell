#include "../../include/minishell.h"

// Corrected Definition: Added t_env *env
t_redir *create_redirection(t_token_type type, char *target, t_data *data)
{
    t_redir *redir = ft_malloc(sizeof(t_redir), 69);
    redir->type = type;
    redir->target = ft_strdup(target);
    redir->expanded_target = NULL;
    redir->quoted_delimiter = 0;
    redir->fd = -1;
    redir->next = NULL;

    if (type == TOKEN_HEREDOC)
    {
        int should_expand;
        char *final_delimiter; // Use a temporary variable

        // Pass env to the analysis function
        if (analyze_heredoc_delimiter(target, &final_delimiter, &should_expand, data))
        {
            redir->quoted_delimiter = !should_expand;
            free(final_delimiter); // Free the temporary variable
        }
    }
    return (redir);
}

void add_redirection(t_redir **redirs, t_redir *new_redir)
{
    t_redir *current;

    if (!*redirs)
    {
        *redirs = new_redir;
        return;
    }
    current = *redirs;
    while (current->next)
        current = current->next;
    current->next = new_redir;
}

int parse_single_redirection(t_token **tokens, t_redir **redirs, t_data *data)
{
    t_token_type redir_type;
    t_redir *new_redir;

    redir_type = (*tokens)->type;
    *tokens = (*tokens)->next;

    if (!*tokens || (*tokens)->type != TOKEN_WORD)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
        return (0);
    }

    new_redir = create_redirection(redir_type, (*tokens)->value, data);
    if (!new_redir)
        return (0);

    add_redirection(redirs, new_redir);
    *tokens = (*tokens)->next; 
    return (1);
}

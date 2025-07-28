#include "../../include/minishell.h"

static t_ast *create_pipe_node(t_ast *left, t_ast *right)
{
    t_ast *node = malloc(sizeof(t_ast));
    if (!node) return (NULL);
    node->type = NODE_PIPE;
    node->args = NULL;
    node->arg_count = 0;
    node->redirs = NULL;
    node->left = left;
    node->right = right;
    return (node);
}

t_ast *parse_pipeline(t_token **tokens, t_data *data)
{
    t_ast *left;
    t_ast *right;

    left = parse_command_with_redirections(tokens, data);
    if (!left) return (NULL);

    while (*tokens && (*tokens)->type == TOKEN_PIPE)
    {
        *tokens = (*tokens)->next; // Consume '|'
        if (!*tokens || (*tokens)->type == TOKEN_PIPE)
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
            free_ast(left);
            return (NULL);
        }
        right = parse_command_with_redirections(tokens, data);
        if (!right)
        {
            free_ast(left);
            return (NULL);
        }
        left = create_pipe_node(left, right);
    }
    return (left);
}

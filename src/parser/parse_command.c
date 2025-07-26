#include "../../include/minishell.h"

static int count_command_args(t_token *tokens)
{
    int count = 0;
    t_token *current = tokens;

    while (current && current->type != TOKEN_PIPE)
    {
        if (is_redirection_token(current->type))
        {
            current = current->next; 
            if (current)
                current = current->next; 
        }
        else
        {
            count++;
            current = current->next;
        }
    }
    return (count);
}

static t_ast *init_command_node(int argc)
{
    t_ast *cmd = malloc(sizeof(t_ast));
    if (!cmd) return (NULL);
    cmd->type = NODE_COMMAND;
    cmd->redirs = NULL;
    cmd->left = NULL;
    cmd->right = NULL;
    cmd->arg_count = argc;
    if (argc > 0)
    {
        cmd->args = malloc(sizeof(char *) * (argc + 1));
        if (!cmd->args)
        {
            free(cmd);
            return (NULL);
        }
    }
    else
        cmd->args = NULL;
    return (cmd);
}

t_ast *parse_command_with_redirections(t_token **tokens, t_env *env)
{
    t_ast *cmd;
    int argc;
    int i = 0;

    argc = count_command_args(*tokens);
    cmd = init_command_node(argc);
    if (!cmd) return (NULL);

    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if (is_redirection_token((*tokens)->type))
        {
            
            if (!parse_single_redirection(tokens, &cmd->redirs, env))
            {
                free_ast(cmd);
                return (NULL);
            }
        }
        else if ((*tokens)->type == TOKEN_WORD)
        {
            if (cmd->args)
			{
                cmd->args[i++] = ft_strdup((*tokens)->value);
			}
            *tokens = (*tokens)->next;
        }
        else
        {
             *tokens = (*tokens)->next;
        }
    }
    if (cmd->args)
	{
        cmd->args[i] = NULL;
	}
  
     if (argc == 0 && !cmd->redirs)
    {
        free_ast(cmd);
        return (NULL);
    }

    return (cmd);
}

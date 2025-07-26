#include "../include/minishell.h"

int g_signal_received = 0;

void handle_line(char *line, t_shell *shell)
{
    t_token *tokens;
    t_ast   *ast;

    tokens = tokenize(line);
    if (tokens && validate_syntax(tokens))
    {
        ast = parse_pipeline(&tokens, shell->env);
        if (ast)
        {
            shell->env->exit_status = exec_ast(ast, shell->env);
            free_ast(ast);
        }
    }
    free_tokens(tokens);
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    char    *line;

    (void)argc;
    (void)argv;
    shell.env = init_env(envp);
    if (!shell.env)
        return (1);
    while (1)
    {
        setup_signals();
        line = readline("minishell$ ");
        if (!line) // Ctrl+D
        {
            printf("exit\n");
            break;
        }
        if (*line)
        {
            add_history(line);
            handle_line(line, &shell);
        }
        free(line);
        if (g_signal_received)
        {
            shell.env->exit_status = g_signal_received;
            g_signal_received = 0;
        }
    }
    free_env(shell.env);
    return (shell.env->exit_status);
}

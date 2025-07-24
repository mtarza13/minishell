
#include "../include/minishell.h"

int	g_signal_received = 0;

int	minishell_loop(t_shell *shell)
{
	char	*line;
	t_token	*tokens;
	t_ast	*ast;

	tokens = NULL;
	ast = NULL;
	while (1)
	{
		setup_signals();
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
		{
			add_history(line);
			tokens = tokenize(line);
			if (tokens && validate_syntax(tokens))
			{
				ast = parse_pipeline(&tokens);
				if (ast)
					exec_ast(ast, shell->env);
			}
			free_tokens(tokens);
			free_ast(ast);
			tokens = NULL;
			ast = NULL;
		}
		free(line);
		if (g_signal_received)
		{
			shell->env->exit_status = g_signal_received;
			g_signal_received = 0;
		}
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.env = init_env(envp);
	shell.pwd = getcwd(NULL, 0);
	shell.oldpwd = NULL;
	shell.stdin_backup = dup(STDIN_FILENO);
	shell.stdout_backup = dup(STDOUT_FILENO);
	if (!shell.env)
	{
		ft_putstr_fd("Error: Failed to initialize environment\n", 2);
		return (1);
	}
	minishell_loop(&shell);
	close(shell.stdin_backup);
	close(shell.stdout_backup);
	free_env(shell.env);
	if (shell.pwd)
		free(shell.pwd);
	if (shell.oldpwd)
		free(shell.oldpwd);
	return (0);
}
#include "mini.h"

void minishell_loop(t_shell *shell)
{
	char *line ;
	t_token *token ;
	t_ast *ast = NULL;
	token = NULL;

	while(1)
	{
		setup_signals();
		line = readline("mini->");
		if(!line)
		{
			printf("exit\n");
			break;
		}
		if(*line)
			add_history(line);
		token = tokenize(line);
		t_token *token_cursor = token;
		ast = parse_pipeline(&token_cursor);
		if(ast)
		{
			exec_ast(as,shell->env);
		}
		free_tokens(token);
		free_ast(ast);
		free(line);
	}
	return ;
}

int main(int argc , char *argv[] , char **envp)
{
	(void)argc;
	(void)argv;
	t_shell shell;
	shell.env = init_env(envp);
	shell.pwd = getcwd(NULL,0);
	shell.oldpwd = NULL;

	if(!shell.env)
	{
		fprintf(stderr,"Error: Failed to initialize environment\n");
		return 1; 
	}
	minishell_loop(&shell);
	free_env(shell.env);

	if(shell.pwd)
		free(shell.pwd);
	if(shell.oldpwd)
		free(shell.pwd);
	return 0;
}

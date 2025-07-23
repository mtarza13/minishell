#include "include/all.h"



// =======Main Loop===


void minishell_loop(t_shell *shell)
{
	char *line;
	  /
}

int	main(int argc , char *argv , char **envp)
{
	(void)argc;
	(void)argv;

	t_shell shell;

	shell.env = init_env(envp);
	shell.pwd = getcwd(NULL,0);

	if(!shell.env)
	{
		fprintf(stderr,"Error: failed to initialize envirnoment");
		return 1;
	}
	minishell_loop(&shell.env);
	
}

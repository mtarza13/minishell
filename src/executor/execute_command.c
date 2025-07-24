
#include "../../include/minishell.h"

static int	execute_external_command(char **args, t_env *env, t_redir *redirs)
{
	pid_t	pid;
	int		status;
	char	**envp;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (!setup_redirections(redirs, env))
			exit(1);
		envp = env_to_array(env);
		execvp(args[0], args);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		ft_free_array(envp);
		exit(errno == EACCES ? PERMISSION_DENIED : COMMAND_NOT_FOUND);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		env->exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
		return (env->exit_status);
	}
	return (1);
}

static int	execute_builtin_with_redirections(char **args, t_env *env,
			t_redir *redirs)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (!setup_redirections(redirs, env))
	{
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		return (1);
	}
	status = execute_builtin(args, env);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (status);
}

int	execute_command(char **args, t_env *env, t_redir *redirs)
{
	if (!args || !args[0])
	{
		if (redirs)
			return (setup_redirections(redirs, env) ? 0 : 1);
		return (0);
	}
	if (is_builtin(args[0]))
		return (execute_builtin_with_redirections(args, env, redirs));
	else
		return (execute_external_command(args, env, redirs));
}
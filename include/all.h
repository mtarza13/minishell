#ifndef ALL_H
#define ALL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <errno.h>

typedef struct s_env
{
	char **envp;
	int count;
	int capacity;
	int exit_status;
}t_env;

typedef struct s_shell
{
	t_env *env;
	char *pwd;
	char *oldpwd;
}t_shell;


t_env  *init_env(char **envp);
#endif

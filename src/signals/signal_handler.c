
#include "../../include/minishell.h"

void	handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal_received = CTRL_C_EXIT;
	}
}

void	handle_heredoc_signal(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_received = CTRL_C_EXIT;
		write(1, "\n", 1);
		exit(CTRL_C_EXIT);
	}
}
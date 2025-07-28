
#include "../../include/minishell.h"

void	handle_signal(int signo)
{
	(void)signo;
	g_signal_received = 1;
	write(2,"\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_heredoc_signal_multi(int signo)
{
	(void)signo;
	g_signal_received = 1;
	// exit(130);
}

void	handle_heredoc_signal(int signo)
{
	(void)signo;
	exit(130);
}

void	handle_sigint_heredoc(int signo)
{
	(void)signo;
	g_signal_received = 1;
	write(2,"^C\n", 3);
}

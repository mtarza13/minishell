
#include "../../include/minishell.h"

void	setup_signals(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_heredoc(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_heredoc_child(void)
{
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	signals_execute(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

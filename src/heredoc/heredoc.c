#include "../../include/minishell.h"

// int analyze_heredoc_delimiter(char *delimiter, char **final_delimiter, int *should_expand, t_env *env)
// {
//     (void)env;
//     if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
//         *should_expand = 0;
//     else
//         *should_expand = 1;
//     *final_delimiter = remove_quotes_advanced(delimiter);
//     if (!*final_delimiter)
//         return (0);
//     return (1);
// }


int setup_heredoc(t_redir *redir, t_env *env)
{
    char *line;
    char *final_delim;
    int should_expand;
    int pipefd[2];
    int line_num;

    if (!analyze_heredoc_delimiter(redir->target, &final_delim, &should_expand,env))
        return (-1);
    if (pipe(pipefd) == -1)
    {
        free(final_delim);
        perror("pipe");
        return (-1);
    }
    signal(SIGINT, handle_heredoc_signal);
    line_num = 1;
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            ft_putstr_fd("minishell: warning: here-document at line ", 2);
            ft_putnbr_fd(line_num, 2);
            ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
            ft_putstr_fd(final_delim, 2);
            ft_putstr_fd("')\n", 2);
            break;
        }
        if (ft_strcmp(line, final_delim) == 0 || g_signal_received)
            break;
        char *line_to_write = line;
        if (should_expand)
            line_to_write = expand_variables_advanced(line, env);
        write(pipefd[1], line_to_write, ft_strlen(line_to_write));
        write(pipefd[1], "\n", 1);
        if (should_expand && line_to_write != line)
            free(line_to_write);
        free(line);
        line_num++;
    }
    if (line)
        free(line);
    free(final_delim);
    close(pipefd[1]);
    if (g_signal_received)
    {
        close(pipefd[0]);
        return (-1);
    }
    setup_signals();
    return (pipefd[0]);
}

#include "../../include/minishell.h"


int analyze_heredoc_delimiter(char *delimiter, char **final_delimiter, int *should_expand, t_env *env)
{
    (void)env; 

    if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
        *should_expand = 0;
    else
        *should_expand = 1;
        
    *final_delimiter = remove_quotes_advanced(delimiter);
    if (!*final_delimiter)
        return (0);
        
    return (1);
}
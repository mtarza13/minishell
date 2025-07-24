
#include "../../include/minishell.h"

int	analyze_heredoc_delimiter(char *delimiter_token,
		char **expanded_delimiter, int *should_expand_content)
{
	int		has_quotes;
	int		i;
	char	*temp_expanded;

	if (!delimiter_token)
		return (0);
	*should_expand_content = 1;
	has_quotes = 0;
	i = 0;
	while (delimiter_token[i])
	{
		if (delimiter_token[i] == '\'' || delimiter_token[i] == '"')
		{
			has_quotes = 1;
			*should_expand_content = 0;
			break ;
		}
		i++;
	}
	temp_expanded = ft_strdup(delimiter_token);
	if (!temp_expanded)
		return (0);
	*expanded_delimiter = remove_quotes_advanced(temp_expanded);
	free(temp_expanded);
	return (1);
}
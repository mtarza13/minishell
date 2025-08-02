#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

typedef struct s_quote_state
{
	int	in_single;
	int	in_double;
	int	in_word;
}	t_quote_state;

typedef struct s_split_state
{
	t_quote_state	state;
	int				i;
	int				word_idx;
}	t_split_state;

int		is_space(char c);
char	*get_env(t_data *data, char *name);
char	*ft_strjoin_free(char *s1, char *s2);
char	*add_char(char *result, char c);
int		has_quotes(const char *s);
char	**split_with_quotes(char *str);
char	*remove_quotes(char *str);
char	*expand_with_quotes(char *str, t_data *data);
int		count_words_with_quotes(char *str);
int		count_total_words(char **args, t_data *data);
char	*expand_dollar(char *str, int *i, t_data *data, int in_single);
void	extract_and_advance(const char *str, int *i, char **words, int *word_idx);
char	**expand_args_array(char **args, t_data *data);
char	*expand_cpy(char **args, t_data *data);

#endif
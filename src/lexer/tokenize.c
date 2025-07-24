
#include "../../include/minishell.h"

static int	handle_quotes_and_escapes(char *input, int *i,
				int *in_single_quote, int *in_double_quote)
{
	if (input[*i] == '\'' && !*in_double_quote)
		*in_single_quote = !*in_single_quote;
	else if (input[*i] == '"' && !*in_single_quote)
		*in_double_quote = !*in_double_quote;
	else if (input[*i] == '\\' && input[*i + 1])
	{
		(*i)++;
		return (1);
	}
	return (0);
}

static int	is_special_char(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<'
		|| c == '>' || c == '(' || c == ')');
}

static int	tokenize_word(char *input, int *i, t_token **tokens)
{
	int		start;
	int		len;
	int		in_single_quote;
	int		in_double_quote;
	char	*word;

	start = *i;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[*i])
	{
		if (!in_single_quote && !in_double_quote && is_special_char(input[*i]))
			break ;
		if (handle_quotes_and_escapes(input, i, &in_single_quote,
				&in_double_quote))
			(*i)++;
		(*i)++;
	}
	len = *i - start;
	if (len > 0)
	{
		word = ft_substr(input, start, len);
		if (!word)
			return (0);
		add_token(tokens, create_token(TOKEN_WORD, word));
		free(word);
	}
	return (1);
}

static int	tokenize_operators(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
	{
		add_token(tokens, create_token(TOKEN_PIPE, "|"));
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			add_token(tokens, create_token(TOKEN_HEREDOC, "<<"));
			*i += 2;
		}
		else
		{
			add_token(tokens, create_token(TOKEN_REDIR_IN, "<"));
			(*i)++;
		}
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(tokens, create_token(TOKEN_REDIR_APPEND, ">>"));
			*i += 2;
		}
		else
		{
			add_token(tokens, create_token(TOKEN_REDIR_OUT, ">"));
			(*i)++;
		}
	}
	else if (input[*i] == '(')
	{
		add_token(tokens, create_token(TOKEN_PAREN_OPEN, "("));
		(*i)++;
	}
	else if (input[*i] == ')')
	{
		add_token(tokens, create_token(TOKEN_PAREN_CLOSE, ")"));
		(*i)++;
	}
	return (1);
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int		i;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (!input[i])
			break ;
		if (is_special_char(input[i]) && input[i] != ' ' && input[i] != '\t')
		{
			if (!tokenize_operators(input, &i, &tokens))
				break ;
		}
		else
		{
			if (!tokenize_word(input, &i, &tokens))
				break ;
		}
	}
	return (tokens);
}
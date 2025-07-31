/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:36:28 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 00:53:02 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	tokenize_operators(char *in, int *i, t_token **toks)
{
	if (in[*i] == '<' || in[*i] == '>' || in[*i] == '|')
	{
		if (in[*i + 1] == '<' || in[*i + 1] == '>')
		{
			if (in[*i + 1] == '<')
				add_token(toks, create_token(TOKEN_HEREDOC, "<<"));
			else if (in[*i + 1] == '>')
				add_token(toks, create_token(TOKEN_REDIR_APPEND, ">>"));
			(*i)++;
		}
		else if (in[*i] == '<')
			add_token(toks, create_token(TOKEN_REDIR_IN, "<"));
		else if (in[*i] == '>')
			add_token(toks, create_token(TOKEN_REDIR_OUT, ">"));
		else if (in[*i] == '|')
			add_token(toks, create_token(TOKEN_PIPE, "|"));
		(*i)++;
		return (1);
	}
	return (0);
}

static int	tokenize_word(char *in, int *i, t_token **toks)
{
	int	start;
	int	in_sq;
	int	in_dq;

	start = *i;
	in_sq = 0;
	in_dq = 0;
	while (in[*i])
	{
		if (in[*i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (in[*i] == '"' && !in_sq)
			in_dq = !in_dq;
		if (!in_sq && !in_dq && (ft_isspace(in[*i]) || is_special_char(in[*i])))
			break ;
		(*i)++;
	}
	if (in_sq || in_dq)
		return (ft_printf("minishell: syntax error: unclosed quote\n"), 0);
	if (*i > start)
		add_token(toks, create_token(TOKEN_WORD, ft_substr(in, start, \
			*i - start)));
	return (1);
}

t_token	*tokenize(char *in)
{
	t_token	*toks;
	int		i;

	if (!in)
		return (NULL);
	toks = NULL;
	i = 0;
	while (in[i])
	{
		while (in[i] && ft_isspace(in[i]))
			i++;
		if (!in[i])
			break ;
		if (is_special_char(in[i]))
		{
			if (!tokenize_operators(in, &i, &toks))
				return (free_tokens(toks), NULL);
		}
		else
		{
			if (!tokenize_word(in, &i, &toks))
				return (free_tokens(toks), NULL);
		}
	}
	return (toks);
}

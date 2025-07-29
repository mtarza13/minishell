#include "../../include/minishell.h"

static int tokenize_operators(char *in, int *i, t_token **toks)
{
    if (in[*i] == '<' && in[*i + 1] == '<')
    {
        add_token(toks, create_token(TOKEN_HEREDOC, "<<"));
        *i += 2;
        return (1);
    }
    if (in[*i] == '>' && in[*i + 1] == '>')
    {
        add_token(toks, create_token(TOKEN_REDIR_APPEND, ">>"));
        *i += 2;
        return (1);
    }
    if (in[*i] == '<')
    {
        add_token(toks, create_token(TOKEN_REDIR_IN, "<"));
        (*i)++;
        return (1);
    }
    if (in[*i] == '>')
    {
        add_token(toks, create_token(TOKEN_REDIR_OUT, ">"));
        (*i)++;
        return (1);
    }
    if (in[*i] == '|')
    {
        add_token(toks, create_token(TOKEN_PIPE, "|"));
        (*i)++;
        return (1);
    }
    return (0);
}

static int tokenize_word(char *in, int *i, t_token **toks)
{
    int start;
    int in_sq; 
    int in_dq; 

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
            break;
        
        (*i)++;
    }

    if (in_sq || in_dq)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
        return (0);
    }

    if (*i > start)
    {
        char *word = ft_substr(in, start, *i - start);
        add_token(toks, create_token(TOKEN_WORD, word));
        // free(word);
    }
    return (1); 
}

t_token *tokenize(char *in)
{
    t_token *toks;
    int i;

    if (!in)
        return (NULL);
    toks = NULL;
    i = 0;
    while (in[i])
    {
        while (in[i] && ft_isspace(in[i]))
            i++;
        if (!in[i])
            break;

        if (is_special_char(in[i]))
        {
            if (!tokenize_operators(in, &i, &toks))
            {
                free_tokens(toks);
                return (NULL);
            }
        }
        else
        {
            if (!tokenize_word(in, &i, &toks))
            {
                free_tokens(toks);
                return (NULL);
            }
        }
    }
    return (toks);
}

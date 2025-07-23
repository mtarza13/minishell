#include "mini.h"

void skip_spaces(char *input , int *i , t_token **token)
{
	while(input[*i] == ' '|| input[*i] == '\t')
		(*i)++;
}

t_token *create_token(t_token_type type, char *value)
{
	t_token *tokens = malloc(sizeof(t_token));
	if(!tokens)
		return (NULL);
	if(value)
	{
		tokens->value = ft_strdup(value);
		//freee
	}
	tokens->type = type;
	tokens->next = NULL;
	return tokens;
}

void add_token(t_token **tokens, t_token *new_token) {
    if (!new_token) return;
    
    if (!*tokens) {
        *tokens = new_token;
        return;
    }
    
    t_token *current = *tokens;
    while (current->next) {
        current = current->next;
    }
    current->next = new_token;
}

int	handle_operators(char *s, int *i, t_token **tokens)
{
	if (s[*i] == '|')
		return (add_token(tokens, create_token(TOKEN_PIPE, "|")), (*i)++, 1);
	if (s[*i] == '<' && s[*i + 1] == '<')
		return (add_token(tokens, create_token(TOKEN_HEREDOC, "<<")),
			(*i) += 2, 1);
	if (s[*i] == '<')
		return (add_token(tokens, create_token(TOKEN_REDIR_IN, "<")),
			(*i)++, 1);
	if (s[*i] == '>' && s[*i + 1] == '>')
		return (add_token(tokens, create_token(TOKEN_REDIR_APPEND, ">>")),
			(*i) += 2, 1);
	if (s[*i] == '>')
		return (add_token(tokens, create_token(TOKEN_REDIR_OUT, ">")),
			(*i)++, 1);
	return (0);
}
void handle_word(char *input, int *i , t_token **token)
{
	int start;
	int sq;
	int dq;
	char *word;
	start = *i;
	while(input[*i])
	{
		if(!sq && !dq && ft_strchr(" \t|<>",input[*i]))
			break;
		if(input[*i] == '\'')
			sq = !sq;
		else if(input[*i] == '"')
			dq = !dq;
		else if(input[*i] == '\\' && input[*i +1])
			(*i)++;
		(*i)++;
	}
	if(*i < start)
	{
		char *word = malloc(*i - start + 1);
		if(word)
		{
				ft_strncpy(word ,input,*i - start);
				word[*i - start] = '\0';
				add_token(token,create_token(TOKEN_WORD ,word));
				free(word);
		}
	}	
}

t_token *tokenize(char *input)
{
	if(!input)
		return (NULL);
	t_token *token;
	token = NULL;
	int i ;
	i = 0;
	while(input[i])
	{
		skip_spaces(input,&i,&token);
		if(!input[i])
			break;
		if(!handle_operators(input,&i,&token))
				handle_word(input, &i ,&token);
	}
	return token;
}
void free_tokens(t_token  *tokens)
{
	while(tokens)
	{
		t_token *tmp = tokens;
		tokens = tokens->next;
		if(tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

#ifndef LEXER_H #define LEXER_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>



typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_HEREDOC,
} t_token_type;

typedef struct s_token {
    char *value;
    t_token_type type;
    struct s_token *next;
}t_token;

t_token *lexer(const char *input);
void print_tokens(t_token *token);
void free_tokens(t_token *token);
int check_syntax_error(t_token *token);

#endif

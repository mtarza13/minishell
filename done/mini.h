#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_type_token {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_HEREDOC
} t_type_token;

typedef struct s_token {
    t_type_token type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_arg {
    char *value;
    struct s_arg *next;
} t_arg;

typedef struct s_redir {
    t_type_token type;
    char *file;
    struct s_redir *next;
} t_redir;

typedef struct s_cmd {
    t_arg *arg;
    t_redir *redir;
    struct s_cmd *next;
} s_cmd;


#endif

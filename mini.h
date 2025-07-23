#ifndef MINI_H
#define MINI_H



# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <errno.h>





typedef enum e_token_type
{
    TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND, TOKEN_HEREDOC,
    TOKEN_EOF, TOKEN_ERROR
}t_token_type;

typedef struct s_token{
	t_token_type type;
	char *value;
	struct s_token *next;
}t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE
}t_node_type;


typedef struct s_redir
{
 	t_token_type type;
	char *target;
	struct s_redir *next;
}t_redir;

typedef struct s_ast
{
	t_node_type type;
	char **arg;
	int arg_count;
	t_redir *redir;
	struct s_ast *left;
	struct s_ast *right;
}t_ast;

typedef struct s_env
{
	char **envp;
	int count;
	int capacity;
	int exit_status;
}t_env;

typedef struct s_shell
{
	t_env *env;
	char *pwd;
	char *oldpwd;
}t_shell;

typedef struct s_expand_context{
	int in_single_quote;
	int in_double_quote;
	int preserve_empty;
	int escape_next;
}t_expand_context;


// Utils
size_t ft_strlen(const char *s);
char *ft_strdup(const char *s);
char *ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2);
int ft_strcmp(const char *s1, const char *s2);
int ft_strncmp(const char *s1, const char *s2, size_t n);
char *ft_strchr(const char *s, int c);
char *ft_strncpy(char *dest, const char *src, size_t n);
void ft_free_array(char **array);
int ft_isalnum(int c);
int ft_isdigit(int c);
int ft_isspace(int c);
int ft_atoi(const char *str);
char *ft_itoa(int n);
int is_valid_identifier(char *str);

// env
t_env *init_env(char **envp);
void free_tokens(t_token  *tokens);
t_token *tokenize(char *input);

#endif

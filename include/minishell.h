#ifndef MINISHELL_H
# define MINISHELL_H

/* C Library Includes */
# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <fcntl.h> // For open()
# include <sys/wait.h> // For waitpid()
# include <sys/stat.h>


/* External Library Includes */
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h" // Adjust path if needed

/* Global Variables for Signals */
extern int g_signal_received;

/* --- Enums and Structs --- */

// Exit codes for signals and errors
# define CTRL_C_EXIT 130
# define COMMAND_NOT_FOUND 127
# define PERMISSION_DENIED 126

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,      // <
    TOKEN_REDIR_OUT,     // >
    TOKEN_HEREDOC,       // <<
    TOKEN_REDIR_APPEND,  // >>
} t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
} t_token;

typedef struct s_redir
{
    t_token_type    type;
    char            *target;
    char            *expanded_target;
    int             quoted_delimiter;
    int             fd;
    struct s_redir  *next;
} t_redir;

typedef enum e_node_type
{
    NODE_COMMAND,
    NODE_PIPE
} t_node_type;

typedef struct s_ast
{
    t_node_type     type;
    char            **args;
    int             arg_count;
    t_redir         *redirs;
    struct s_ast    *left;
    struct s_ast    *right;
} t_ast;

typedef struct s_env
{
	int					count;
	char				*key;
	char				*value;
	struct s_env		*next;
} t_env;

typedef struct s_data
{
	char	**envp;
	int		status;
    t_env	*env;
} t_data;

typedef struct s_expand_context
{
    int in_single_quote;
    int in_double_quote;
} t_expand_context;

typedef struct s_mem
{
	void				*ptr;
	struct s_mem		*next;
}	t_mem;

/* --- Function Prototypes --- */

// --- Lexer (lexer/) ---
t_token *tokenize(char *in);
t_token *create_token(t_token_type type, char *value);
void    add_token(t_token **tokens, t_token *new_token);
void    free_tokens(t_token *tokens);
int     is_special_char(char c);

// --- Parser (parser/) ---
t_ast   *parse_pipeline(t_token **tokens, t_data *data);
t_ast   *parse_command_with_redirections(t_token **tokens, t_data *data);
int     parse_single_redirection(t_token **tokens, t_redir **redirs, t_data *data);
int     validate_syntax(t_token *tokens);
void	minishell(t_data *data);

// --- Executor (executor/) ---
int     exec_ast(t_ast *ast, t_data *data);
int     execute_command(char **args, t_data *data, t_redir *redirs);
int     execute_builtin(char **args, t_data *data);
int     is_builtin(char *cmd);
int     setup_redirections(t_redir *redirs, t_data *data);
int     is_redirection_token(t_token_type type);
char	*filename(char *cmd, t_data *data);

// --- Heredoc (heredoc/) ---
int     setup_heredoc(t_redir *redir, t_env *env);
int     analyze_heredoc_delimiter(char *delimiter, char **final_delimiter, int *should_expand, t_data *data);
int		heredoc_check_single(t_redir *current, t_env *env);
int		heredoc_check_multi(t_redir *current, t_env *env);
int		heredoc_check(t_token *token, t_data *data);

// --- Expansion (env/) ---
char    *expand_variables_advanced(char *str, t_data *data);
char    **expand_args_professional(char **args, t_data *data);
char    *get_env_value(char *name, t_data *data);
int     set_env_value(t_env *env, char *name, char *value);
int     unset_env_value(t_env *env, char *name);
t_env   *init_env(char **envp);
char    **env_to_array(t_data *data);
t_env	*new_env_node(char *key, char *value);
void	add_env_node(t_env **env, t_env *new);
char	**ft_envp(t_env *env);
int		list_len(t_env *env);

// --- Builtins (builtins/) ---
int     builtin_cd(char **args, t_data *data);
int     builtin_echo(char **args);
int     builtin_env(char **args, t_data *data);
int     builtin_exit(char **args, t_data *data);
int     builtin_export(char **args, t_data *data);
int     builtin_pwd(char **args, t_data *data);
int     builtin_unset(char **args, t_data *data);

// --- Signals (signals/) ---
void    setup_signals(void);
void    handle_signal(int signo);
void    handle_heredoc_signal(int signo);
void	handle_sigint_heredoc(int signo);
void	handle_heredoc_signal_multi(int signo);
void	signals_heredoc(void);
void	signals_heredoc_child(void);
void	signals_execute(void);

// --- Utils (utils/) ---
void    free_ast(t_ast *ast);
void    free_redirections(t_redir *redirs);
void    ft_free_array(char **array);
void    free_env(t_env *env);
int     ft_isspace(int c);
int     is_valid_identifier(char *str);
char    *remove_quotes_advanced(char *str);
char    *append_char_dynamic(char *str, char c, int *len, int *capacity);
void	*ft_malloc(size_t size, int flag);
void	free_data(void);
void	ft_printf(const char *s, ...);

#endif

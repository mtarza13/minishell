
#ifndef MINISHELL_H
# define MINISHELL_H

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
# include "../libft/libft.h"

# define SYNTAX_ERROR 2
# define COMMAND_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define CTRL_C_EXIT 130
# define CTRL_BACKSLASH_EXIT 131
# define MAX_HEREDOC_SIZE 1000000

extern int	g_signal_received;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_EOF,
	TOKEN_ERROR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE
}	t_node_type;

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	char			*expanded_target;
	int				quoted_delimiter;
	int				fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast
{
	t_node_type		type;
	char			**args;
	int				arg_count;
	t_redir			*redirs;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_env
{
	char	**envp;
	int		count;
	int		capacity;
	int		exit_status;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	char	*pwd;
	char	*oldpwd;
	int		stdin_backup;
	int		stdout_backup;
}	t_shell;

typedef struct s_expand_context
{
	int	in_single_quote;
	int	in_double_quote;
	int	preserve_empty;
	int	escape_next;
}	t_expand_context;

/* ************************************************************************** */
/*                               MAIN                                         */
/* ************************************************************************** */
int		minishell_loop(t_shell *shell);

/* ************************************************************************** */
/*                               UTILS                                        */
/* ************************************************************************** */
char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2);
int		ft_isspace(int c);
char	*append_char_dynamic(char *str, char c, int *len, int *capacity);
int		is_valid_identifier(char *str);
void	ft_free_array(char **array);

/* ************************************************************************** */
/*                               ENVIRONMENT                                  */
/* ************************************************************************** */
t_env	*init_env(char **envp);
void	free_env(t_env *env);
char	*get_env_value(t_env *env, char *name);
int		set_env_value(t_env *env, char *name, char *value);
int		unset_env_value(t_env *env, char *name);
char	**env_to_array(t_env *env);

/* ************************************************************************** */
/*                            VARIABLE EXPANSION                              */
/* ************************************************************************** */
char	*expand_variables_advanced(char *str, t_env *env);
char	**expand_args_professional(char **args, t_env *env);
char	*remove_quotes_advanced(char *str);
char	**split_unquoted_words(char *str);

/* ************************************************************************** */
/*                               LEXER                                        */
/* ************************************************************************** */
t_token	*create_token(t_token_type type, char *value);
void	add_token(t_token **tokens, t_token *new_token);
t_token	*tokenize(char *input);
void	free_tokens(t_token *tokens);

/* ************************************************************************** */
/*                               PARSER                                       */
/* ************************************************************************** */
t_ast	*parse_pipeline(t_token **tokens);
t_ast	*parse_command_with_redirections(t_token **tokens);
int		validate_syntax(t_token *tokens);
void	free_ast(t_ast *ast);
void	free_redirections(t_redir *redirs);
int		parse_single_redirection(t_token **tokens, t_redir **redirs);

/* ************************************************************************** */
/*                            REDIRECTIONS                                    */
/* ************************************************************************** */
t_redir	*create_redirection(t_token_type type, char *target);
void	add_redirection(t_redir **redirs, t_redir *new_redir);
int		setup_redirections(t_redir *redirs, t_env *env);
int		is_redirection_token(t_token_type type);

/* ************************************************************************** */
/*                               HEREDOC                                      */
/* ************************************************************************** */
int		setup_heredoc_complete(t_redir *redir, t_env *env);
int		analyze_heredoc_delimiter(char *delimiter_token,
			char **expanded_delimiter, int *should_expand_content);

/* ************************************************************************** */
/*                               EXECUTOR                                     */
/* ************************************************************************** */
int		exec_ast(t_ast *ast, t_env *env);
int		execute_command(char **args, t_env *env, t_redir *redirs);
int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_env *env);

/* ************************************************************************** */
/*                               BUILTINS                                     */
/* ************************************************************************** */
int		builtin_cd(char **args, t_env *env);
int		builtin_echo(char **args, t_env *env);
int		builtin_env(char **args, t_env *env);
int		builtin_exit(char **args, t_env *env);
int		builtin_export(char **args, t_env *env);
int		builtin_pwd(char **args, t_env *env);
int		builtin_unset(char **args, t_env *env);

/* ************************************************************************** */
/*                               SIGNALS                                      */
/* ************************************************************************** */
void	setup_signals(void);
void	handle_signal(int signo);
void	handle_heredoc_signal(int signo);

#endif

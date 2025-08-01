/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:46:50 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/01 06:11:38 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern int			g_signal_received;

# define CTRL_C_EXIT 130
# define COMMAND_NOT_FOUND 127
# define PERMISSION_DENIED 126

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_REDIR_APPEND,
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	char			*expanded_target;
	int				quoted_delimiter;
	int				fd;
	struct s_redir	*next;
}					t_redir;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE
}					t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	char			**args;
	int				arg_count;
	t_redir			*redirs;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct s_env
{
	int				count;
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	char			**envp;
	int				status;
	t_env			*env;
}					t_data;

typedef struct s_expand_context
{
	int				in_single_quote;
	int				in_double_quote;
}					t_expand_context;

typedef struct s_mem
{
	void			*ptr;
	struct s_mem	*next;
}					t_mem;

/* --- Function  EXpnatoin Prototypes --- */
int					is_ifs_char(char c);
int					should_expand_dollar(char *str, int i, int sq);
char				*append_to_result(char *result, char *to_append);
char				*extract_var_name(char *str, int *i);
char				*expand_variable(char *str, int *i, t_data *data);
char				*handle_quote(char *str, int *i, int *sq, int *dq);
char				*remove_quotes(char *str);
char				*expand_string(char *str, t_data *data);
char				*expand_arg_list(char *str, t_data *data);

char				**split_expanded_string(char *expanded_str);
char				**expand_arg_array(char **args, t_data *data);
// expov2
int	count_total_words(char **args, t_data *data);
int					count_split_words(char **split);
// expo
void				init_unquote_vars(int *len, int *i, int *in_quotes,
						char *quote_char);
void				init_unquote_vars(int *len, int *i, int *in_quotes,
						char *quote_char);
void				init_remove_vars(int *i, int *j, int *in_quotes,
						char *quote_char);
int					calculate_unquoted_length(char *str);
void				init_unquote_vars(int *len, int *i, int *in_quotes,
						char *quote_char);
void				update_quote_state(char c, int *in_quotes,
						char *quote_char);

// --- Lexer (lexer/) ---
t_token				*tokenize(char *in);
t_token				*create_token(t_token_type type, char *value);
void				add_token(t_token **tokens, t_token *new_token);
void				free_tokens(t_token *tokens);
int					is_special_char(char c);

// --- Parser (parser/) ---
t_ast				*parse_pipeline(t_token **tokens, t_data *data);
t_ast				*parse_command_with_redirections(t_token **tokens,
						t_data *data);
int					parse_single_redirection(t_token **tokens, t_redir **redirs,
						t_data *data);
int					validate_syntax(t_token *tokens, t_data *data);
void				minishell(t_data *data);

// --- Executor (executor/) ---
int					exec_ast(t_ast *ast, t_data *data);
int					execute_command(char **args, t_data *data, t_redir *redirs);
int					execute_builtin(char **args, t_data *data);
int					is_builtin(char *cmd);
int					setup_redirections(t_redir *redirs, t_data *data);
int					is_redirection_token(t_token_type type);
char				*filename(char *cmd, t_data *data);
int					handle_input_redirection(t_redir *redir, t_data *data,
						int f);
int					handle_output_redirection(t_redir *redir, t_data *data,
						int f);

// --- Heredoc (heredoc/) ---
int					setup_heredoc(t_redir *redir, t_env *env);
int					analyze_heredoc_delimiter(char *delimiter,
						char **final_delimiter, int *should_expand,
						t_data *data);
int					heredoc_check_single(t_redir *current, t_env *env);
int					heredoc_check_multi(t_redir *current, t_env *env);
int					heredoc_check(t_token *token, t_data *data);

// --- Expansion (env/) ---
char				*get_env_value(char *name, t_data *data);
int					set_env_value(t_env *env, char *name, char *value);
int					unset_env_value(t_env *env, char *name);
t_env				*init_env(char **envp);
char				**env_to_array(t_data *data);
t_env				*new_env_node(char *key, char *value);
void				add_env_node(t_env **env, t_env *new);
char				**ft_envp(t_env *env);
int					list_len(t_env *env);
void				update_env_node(t_env *env, char *key, char *value);
int					is_env_char(char c);

// --- Builtins (builtins/) ---
int					builtin_cd(char **args, t_data *data);
int					builtin_echo(char **args);
int					builtin_env(char **args, t_data *data);
int					builtin_exit(char **args, t_data *data);
int					builtin_export(char **args, t_data *data);
int					builtin_pwd(char **args, t_data *data);
int					builtin_unset(char **args, t_data *data);

// --- Signals (signals/) ---
void				setup_signals(void);
void				handle_signal(int signo);
void				handle_heredoc_signal(int signo);
void				handle_sigint_heredoc(int signo);
void				handle_heredoc_signal_multi(int signo);
void				signals_heredoc(void);
void				signals_heredoc_child(void);
void				signals_execute(void);

// --- Utils (utils/) ---
void				free_ast(t_ast *ast);
void				free_redirections(t_redir *redirs);
void				ft_free_array(char **array);
void				free_env(t_env *env);
int					ft_isspace(int c);
int					is_valid_identifier(char *str);
char				*remove_quotes_advanced(char *str);
char				*append_char_dynamic(char *str, char c, int *len,
						int *capacity);
void				*ft_malloc(size_t size, int flag);
void				free_data(void);
void				ft_printf(const char *s, ...);

#endif

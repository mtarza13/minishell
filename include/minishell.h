/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:46:50 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/09 00:06:19 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
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

typedef enum e_type_token {
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_type_token;

typedef struct s_mem
{
	void			*ptr;
	struct s_mem	*next;
}					t_mem;

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
	bool			pipes;
	int				**pipe;
	int				cc;
	t_env			*env;
}					t_data;

typedef struct s_token
{
	t_type_token	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_arg
{
	char			*value;
	struct s_arg	*next;
}	t_arg;

typedef struct s_redir
{
	t_type_token	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	t_arg			*arg;
	char			**args;
	t_redir			*redir;
	t_data			*data;
	int				in;
	int				out;
	struct s_cmd	*next;
}	t_cmd;

void		*ft_malloc(size_t size, int flag);
void		ft_printf(const char *s, ...);
void		minishell(t_data *data);
char		**ft_envp(t_env *env);
t_env		*init_env(char **env);
void		setup_signals(void);
void		sigint_handle(int sig);
void		signals_heredoc(void);
void		signals_heredoc_child(void);
void		heredoc_sigint_handle(int sig);
void		heredoc_child_handle(int sig);
void		signals_execute(void);
void		signals_child(void);
t_token		*tokenizer(char *line);
int			valid_input(t_token *token, t_data *data);
t_cmd		*parse_cmd(t_token **token, t_data *data);
int			ft_isspace(int c);
int			is_operater_char(char in);
t_token		*add_token(t_token *token, t_token *new);
t_token		*creat_token(char *line, t_type_token type);
int			is_redir(t_type_token type);
t_cmd		*parser(t_token *token, t_data *data);
int			execute(t_cmd *cmd, t_data *data);
int			execute_single(t_cmd *cmd, t_data *data);
int			execute_multi(t_cmd *cmd, t_data *data);
int			wait_childs(t_data *data, int *pid);
int			redirs(t_cmd *cmd, int index);
char		*filename(char *cmd, t_data *data);
void		open_pipes(t_cmd *cmd);
void		close_pipes(t_data *data);
void		free_n_exit(int status);
void		if_redirs(t_cmd *cmd);
int			is_builtin(char *cmd);
int			execute_builtin(char **args, t_data *data);
void		exit_status(int status);
int			ft_cd(char **args, t_data *data);
int			ft_echo(char **args);
int			ft_env(t_data *data);
int			ft_exit(char **args, t_data *data);
int			ft_unset(char **args, t_data *data);
int			ft_pwd(t_data *data);
int			ft_export(char **args, t_data *data);
void		update_env_node(t_env *env, char *key, char *value);
int			is_valid_key(char *key, t_data *data);
int			is_env_char(char c);
void		add_env_node(t_env **env, t_env *new);
t_env		*new_env_node(char *key, char *value);
char		*get_env_value(char *var, t_data *data);
void		update_env(t_data *data, char *key, char *value);
void		clean_up(t_token *token);
void		free_data(void);
int			list_len(t_env *env);
int			heredoc_check(t_token *token, t_data *data);
void		pipe_redir(t_cmd *cmd, int index);
char		*expand_arg_list(char **args, t_data *data);

char		*expand_env_value(char *str, int *i, t_data *data);
char		*remove_quote(char *str);
char		*expand_string(char *arg, t_data *data);
char		**expand_arg_array(char **arg, t_data *data);
char		*expand_arg_list(char **args, t_data *data);
char		**split_word(char *str);
int			split_count_word(char *str);
int			has_quote(char *str);
int			count_total_word(char **arg, t_data *data);

char		*get_env(t_data *data, char *value);
char		*add_char(char *result, char c);
int			is_space(char c);
void		free_array(char **array);
#endif

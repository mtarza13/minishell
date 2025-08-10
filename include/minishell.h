/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:46:50 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/10 21:15:10 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../libft/libft.h"

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
	char			*key;
	char			*value;
	struct s_env	*next;
	int				count;
}					t_env;

typedef struct s_data
{
	char			**envp;
	int				**pipe;
	t_env			*env;
	int				status;
	int				cc;
	bool			pipes;
}					t_data;

typedef struct s_token
{
	char			*value;
	struct s_token	*next;
	t_type_token	type;
}	t_token;

typedef struct s_arg
{
	char			*value;
	struct s_arg	*next;
}	t_arg;

typedef struct s_redir
{
	char			*file;
	struct s_redir	*next;
	t_type_token	type;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_arg			*arg;
	t_redir			*redir;
	t_data			*data;
	struct s_cmd	*next;
	int				in;
	int				out;
}	t_cmd;

extern int			g_signal_received;

int					valid_input(t_token *token, t_data *data);
int					ft_isspace(int c);
int					is_operater_char(char in);
int					is_redir(t_type_token type);
int					execute(t_cmd *cmd, t_data *data);
int					execute_single(t_cmd *cmd, t_data *data);
int					execute_multi(t_cmd *cmd, t_data *data);
int					wait_childs(t_data *data, int *pid);
int					redirs(t_cmd *cmd, int index);
int					is_builtin(char *cmd);
int					execute_builtin(char **args, t_data *data);
int					ft_cd(char **args, t_data *data);
int					ft_echo(char **args);
int					ft_env(t_data *data);
int					ft_exit(char **args, t_data *data);
int					ft_unset(char **args, t_data *data);
int					ft_pwd(t_data *data);
int					ft_export(char **args, t_data *data);
int					is_valid_key(char *key, t_data *data);
int					is_env_char(char c);
int					list_len(t_env *env);
int					heredoc_check(t_token *token, t_data *data);
int					split_count_word(char *str);
int					has_quote(char *str);
int					count_total_word(char **arg, t_data *data);
int					is_space(char c);
int					check_word_utils(t_token **token, t_data *data, t_cmd *cmd);

char				**ft_envp(t_env *env);
char				*filename(char *cmd, t_data *data);
char				*expand_arg_list(char **args, t_data *data);
char				*get_env_value(char *var, t_data *data);
char				*expand_env_value(char *str, int *i, t_data *data);
char				*remove_quote(char *str);
char				*expand_string(char *arg, t_data *data);
char				**expand_arg_array(char **arg, t_data *data);
char				*expand_arg_list(char **args, t_data *data);
char				**split_word(char *str);
char				*get_env(t_data *data, char *value);
char				*add_char(char *result, char c);

void				*ft_malloc(size_t size, int flag);
void				ft_printf(const char *s, ...);
void				minishell(t_data *data);
void				setup_signals(void);
void				sigint_handle(int sig);
void				signals_heredoc(void);
void				signals_heredoc_child(void);
void				heredoc_sigint_handle(int sig);
void				heredoc_child_handle(int sig);
void				signals_execute(void);
void				signals_child(void);
void				open_pipes(t_cmd *cmd);
void				close_pipes(t_data *data);
void				free_n_exit(int status);
void				if_redirs(t_cmd *cmd);
void				exit_status(int status, int f);
void				update_env_node(t_env *env, char *key, char *value);
void				add_env_node(t_env **env, t_env *new);
void				update_env(t_data *data, char *key, char *value);
void				clean_up(t_token *token);
void				free_data(void);
void				pipe_redir(t_cmd *cmd, int index);
void				p_syntax_err(char *s);
void				free_array(char **array);
void				if_nocmd(t_cmd *cmd);

t_env				*init_env(char **env);
t_env				*new_env_node(char *key, char *value);

t_token				*tokenizer(char *line);
t_token				*add_token(t_token *token, t_token *new);
t_token				*creat_token(char *line, t_type_token type);

t_arg				*add_arg(t_arg *head_arg, char *new_cmd);

t_cmd				*parse_cmd(t_token **token, t_data *data);
t_cmd				*parser(t_token *token, t_data *data);

#endif

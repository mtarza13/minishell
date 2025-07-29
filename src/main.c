/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:58:43 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/30 00:34:05 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_signal_received = 0;

void	free_data(void)
{
	ft_malloc(0, 0);
	rl_clear_history();
}

void handle_line(char *line, t_data *data)
{
    t_token *tokens;
    t_ast   *ast;

    tokens = tokenize(line);
	free(line);
    if (tokens && validate_syntax(tokens))
    {
		if (!heredoc_check(tokens, data))
			return (free_tokens(tokens), (void)1);
        ast = parse_pipeline(&tokens, data);
        if (ast)
        {
			signals_execute();
            data->status = exec_ast(ast, data);
            free_ast(ast);
        }
    }
    free_tokens(tokens);
}

static int	list_len(t_env *env)
{
	int		i;
	t_env	*tmp;

	i = 1;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**ft_envp(t_env *env)
{
	int		i;
	t_env	*tmp;
	char	**envp;

	i = 0;
	tmp = env;
	envp = (char **)ft_malloc(sizeof(char *) * list_len(env), 43);
	while (tmp)
	{
		envp[i] = (char *)ft_malloc(sizeof(char) * (ft_strlen(tmp->key) + \
		ft_strlen(tmp->value) + 2), 65);
		ft_strncpy(envp[i], tmp->key, ft_strlen(tmp->key) + 1);
		ft_strncat(envp[i], "=", ft_strlen(tmp->key) + 2);
		ft_strncat(envp[i], tmp->value, ft_strlen(tmp->value) + \
		ft_strlen(tmp->key) + 2);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	add_env_node(t_env **env, t_env *new)
{
	t_env	*temp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	temp = *env;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*new;

	new = ft_malloc(sizeof(t_env), -42);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	parse_env_var(t_env **env, char *env_str)
{
	char	*key;
	char	*value;
	char	*equals;
	t_env	*new;

	equals = ft_strchr(env_str, '=');
	if (!equals)
	{
		key = ft_strdup(env_str);
		value = NULL;
	}
	else
	{
		key = ft_strndup(env_str, equals - env_str);
		value = ft_strdup(equals + 1);
	}
	new = new_env_node(key, value);
	if (new)
		add_env_node(env, new);
}

t_env	*init_env(char **env)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		parse_env_var(&env_list, env[i]);
		i++;
	}
	return (env_list);
}

void	init_data(t_data *data, char **envp)
{
	rl_outstream = stderr;
	data->status = 0;
	data->env = init_env(envp);
	data->envp = ft_envp(data->env);
}

void	minishell(t_data *data)
{
	char	*input;

    while (1)
    {
        setup_signals();
        input = readline("minishell$ ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (g_signal_received)
        {
            data->status = CTRL_C_EXIT;
            g_signal_received = 0;
        }
        if (*input)
        {
            add_history(input);
            handle_line(input, data);
        }
        // free(input);
    }
    // free_env(shell.env);
}

int main(int argc, char **argv, char **envp)
{
    t_data	*data;
	int		exit_status;

    (void)argc;
    (void)argv;
	data = ft_malloc(sizeof(t_data), 1337);
	init_data(data, envp);
	minishell(data);
	exit_status = data->status;
	ft_malloc(0, 0);
    return (exit_status);
}

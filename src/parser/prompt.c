/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:17:11 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/10 15:38:07 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

t_cmd	*build_commands(t_cmd *cmd)
{
	t_cmd *(tmp_cmd);
	t_arg *(tmp_arg);
	int (i);
	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		tmp_arg = tmp_cmd->arg;
		i = 0;
		while (tmp_arg)
		{
			i++;
			tmp_arg = tmp_arg->next;
		}
		tmp_cmd->args = ft_malloc(sizeof(char *) * (i + 1), 1337);
		i = 0;
		tmp_arg = tmp_cmd->arg;
		while (tmp_arg)
		{
			tmp_cmd->args[i++] = tmp_arg->value;
			tmp_arg = tmp_arg->next;
		}
		tmp_cmd->args[i] = NULL;
		tmp_cmd = tmp_cmd->next;
	}
	return (cmd);
}

void	handle_line(char *line, t_data *data)
{
	t_token *(tokens);
	t_cmd *(cmd);
	cmd = NULL;
	tokens = tokenizer(line);
	free(line);
	if (tokens)
	{
		if (valid_input(tokens, data))
		{
			if (!heredoc_check(tokens, data))
				return (clean_up(tokens), (void)0);
			cmd = parser(tokens, data);
			if (cmd)
			{
				data->cc = count_cmds(cmd);
				cmd = build_commands(cmd);
				data->envp = ft_envp(data->env);
				signals_execute();
				data->status = execute(cmd, data);
			}
		}
		else
			data->status = 2;
		clean_up(tokens);
	}
}

void	minishell(t_data *data)
{
	char	*input;

	while (true)
	{
		setup_signals();
		data->pipes = false;
		input = readline("% ");
		if (g_signal_received)
		{
			data->status = g_signal_received;
			g_signal_received = 0;
		}
		if (!input)
			break ;
		if (*input)
			(add_history(input), handle_line(input, data));
		else
			free(input);
	}
	ft_printf("exit\n");
}

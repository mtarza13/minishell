/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:17:11 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/07 20:37:53 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_line(char *line, t_data *data)
{
	t_token	*tokens;
	t_cmd	*cmd;

	tokens = tokenize(line);
	free(line);
	if (tokens && validate_syntax(tokens, data))
	{
		if (!heredoc_check(tokens, data))
			return (clean_up(tokens), (void)0);
		cmd = parse_pipeline(tokens, data);
		if (cmd)
		{
			signals_execute();
			data->status = execute(cmd, data);
			// free_ast(cmd);
		}
	}
	clean_up(tokens);
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
			data->status = CTRL_C_EXIT;
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

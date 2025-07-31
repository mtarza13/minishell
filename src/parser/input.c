/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:17:11 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 04:36:06 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_line(char *line, t_data *data)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = tokenize(line);
	free(line);
	if (tokens && validate_syntax(tokens, data))
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
			break ;
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
		else
			free(input);
	}
}

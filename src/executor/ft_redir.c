/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 22:54:05 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/09 00:56:04 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_error(char *str, int err)
{
	if (err == 1)
		ft_printf("minishell: %s: Permission denied\n", str);
	else if (err == 2)
		ft_printf("minishell: %s: No such file or directory\n", str);
}

void	check_access(char *file)
{
	if (access(file, F_OK) == -1)
		print_error(file, 2);
	else
		print_error(file, 1);
}

int	check_in(t_cmd *cmd, t_redir *redir)
{
	if (cmd->in != -1)
		close(cmd->in);
	if (redir->type == REDIR_HEREDOC || redir->type == REDIR_IN)
		cmd->in = open(redir->file, O_RDONLY);
	if (cmd->in == -1)
	{
		check_access(redir->file);
		return (1);
	}
	return (0);
}

int	check_out(t_cmd *cmd, t_redir *redir)
{
	if (cmd->out != -1)
		close(cmd->out);
	if (redir->type == REDIR_OUT)
		cmd->out = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (redir->type == REDIR_APPEND)
		cmd->out = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (cmd->out == -1)
	{
		check_access(redir->file);
		return (1);
	}
	return (0);
}

int	redirs(t_cmd *cmd, int index)
{
	t_redir	*tmp;

	tmp = cmd->redir;
	if (!tmp && index != -1 && cmd->data->cc > 1)
		pipe_redir(cmd, index);
	while (tmp)
	{
		if (tmp->type == REDIR_HEREDOC || tmp->type == REDIR_IN)
		{
			if (check_in(cmd, tmp))
				return (1);
		}
		else if (tmp->type == REDIR_APPEND || tmp->type == REDIR_OUT)
		{
			if (check_out(cmd, tmp))
				return (1);
		}
		tmp = tmp->next;
	}
	if (cmd->in == -1)
		cmd->in = STDIN_FILENO;
	if (cmd->out == -1)
		cmd->out = STDOUT_FILENO;
	return (0);
}

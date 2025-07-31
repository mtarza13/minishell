/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 02:43:22 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/31 02:44:39 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_redirections(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		tmp = redirs;
		redirs = redirs->next;
		if (tmp->type == TOKEN_HEREDOC)
			unlink(tmp->target);
	}
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->redirs)
		free_redirections(ast->redirs);
}

void	ft_free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		i++;
}

void	free_env(t_env *env)
{
	if (!env)
		return ;
}

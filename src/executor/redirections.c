/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 22:54:05 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 20:45:22 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int handle_input_redirection(t_redir *redir, t_data *data)
{
    char **expanded;
    int fd;
    int word_count = 0;

    // if (redir->type == TOKEN_HEREDOC)
    // {
    //     fd = setup_heredoc(redir, env);
    //     if (fd < 0) return (0);
    //     dup2(fd, STDIN_FILENO);
    //     close(fd);
    //     return (1);
    // }
    
    expanded = expand_args_professional(&redir->target, data);
    if (!expanded) {
        ft_putstr_fd("minishell 2: ambiguous redirect\n", 2);
        return (0);
    }
    while(expanded[word_count]) word_count++;
    if (word_count != 1) {
        ft_free_array(expanded);
        ft_putstr_fd("minishell 1: ambiguous redirect\n", 2);
        return (0);
    }

    fd = open(expanded[0], O_RDONLY);
    if (fd == -1) {
        perror(expanded[0]);
        ft_free_array(expanded);
        return (0);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    ft_free_array(expanded);
    return (1);
}

static int handle_output_redirection(t_redir *redir, t_data *data)
{
    char **expanded;
    int fd;
    int flags;
    int word_count = 0;

    expanded = expand_args_professional(&redir->target, data);
    if (!expanded) {
        ft_putstr_fd("minishell: ambiguous redirect\n", 2);
        return (0);
    }
    while(expanded[word_count]) word_count++;
    if (word_count != 1) {
        ft_free_array(expanded);
        ft_putstr_fd("minishell: ambiguous redirect\n", 2);
        return (0);
    }

    if (redir->type == TOKEN_REDIR_APPEND)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    
    fd = open(expanded[0], flags, 0644);
    if (fd == -1) {
        perror(expanded[0]);
        ft_free_array(expanded);
        return (0);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    ft_free_array(expanded);
    return (1);
}

static t_redir *find_last_input_redir(t_redir *redirs)
{
    t_redir *last_input = NULL;
    while (redirs)
    {
        if (redirs->type == TOKEN_REDIR_IN || redirs->type == TOKEN_HEREDOC)
        {
            last_input = redirs;
        }
        redirs = redirs->next;
    }
    return (last_input);
}

int setup_redirections(t_redir *redirs, t_data *data)
{
    t_redir	*current = redirs;
    t_redir	*last_input_redir = find_last_input_redir(redirs);
    // int		heredoc_fd;

	// if (heredoc_check_multi(current, env))
	// 	return (0);
    while (current)
    {
        // if (current->type == TOKEN_HEREDOC)
        // {
        //     heredoc_fd = setup_heredoc(current, env);
        //     if (heredoc_fd < 0)
        //         return (0);
        //     if (current != last_input_redir)
        //         close(heredoc_fd);
        //     else
        //         dup2(heredoc_fd, STDIN_FILENO);
        // }
        if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
        {
            if (current == last_input_redir)
            {
				if (!handle_input_redirection(current, data))
					return (0);
            }
        }
        else if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND)
        {
            if (!handle_output_redirection(current, data))
                return (0);
        }
        current = current->next;
    }
    return (1);
}
int is_redirection_token(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

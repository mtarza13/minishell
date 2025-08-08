/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:33:36 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/08 16:45:49 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	analyze_heredoc_delimiter(char *delimiter, char **final_delimiter, \
// 	int *should_expand, t_data *data)
// {
// 	(void)data;
// 	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
// 		*should_expand = 0;
// 	else
// 		*should_expand = 1;
// 	*final_delimiter = remove_quotes_advanced(delimiter);
// 	if (!*final_delimiter)
// 		return (0);
// 	return (1);
// }

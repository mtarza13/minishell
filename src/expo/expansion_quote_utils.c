/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quote_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:41:39 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/01 05:35:08 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	should_expand_dollar(char *str, int i, int sq)
{
	if (sq)
		return (0);
	if (i > 0 && str[i - 1] == '\\')
		return (0);
	return (1);
}

char	*append_to_result(char *result, char *to_append)
{
	char	*new_result;

	if (!result || !to_append)
		return (result);
	new_result = ft_strjoin(result, to_append);
	return (new_result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expo_v2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:48:40 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/01 05:35:36 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_split_words(char **split)
{
	int	count;

	count = 0;
	while (split && split[count])
		count++;
	return (count);
}

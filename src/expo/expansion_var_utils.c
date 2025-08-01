/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_var_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 04:26:33 by mtarza            #+#    #+#             */
/*   Updated: 2025/08/01 05:10:06 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_from_data(char *var_name, t_data *data)
{
	t_env	*current;

	if (!var_name || !data || !data->env)
		return (ft_strdup(""));
	current = data->env;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
		{
			if (current->value)
				return (ft_strdup(current->value));
			else
				return (ft_strdup(""));
		}
		current = current->next;
	}
	return (ft_strdup(""));
}

char	*expand_exit_status(t_data *data)
{
	return (ft_itoa(data->status));
}

char	*extract_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*var_name;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (!ft_isalpha(str[*i]) && str[*i] != '_')
		return (NULL);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	var_name = ft_substr(str, start, len);
	return (var_name);
}

char	*expand_variable(char *str, int *i, t_data *data)
{
	char	*var_name;
	char	*value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (expand_exit_status(data));
	}
	var_name = extract_var_name(str, i);
	if (!var_name)
		return (ft_strdup("$"));
	if (ft_strcmp(var_name, "?") == 0)
		value = expand_exit_status(data);
	else
		value = get_env_from_data(var_name, data);
	return (value);
}

int	is_ifs_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

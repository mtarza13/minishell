/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 10:09:52 by yabarhda          #+#    #+#             */
/*   Updated: 2025/07/28 21:53:18 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_strchr_ex(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (0);
		i++;
	}
	return (1);
}

void	ft_strncat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	if ((!src && size == 0) || (!dest && size == 0))
		return ;
	i = 0;
	j = ft_strlen(dest);
	if (size == 0 || j > size)
		return ;
	while (j + i < size - 1 && src[i])
	{
		dest[j + i] = src[i];
		i++;
	}
	dest[j + i] = '\0';
}

char	*ft_strjoin_ex(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	char	*dest;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1) + 1;
	s2_len = ft_strlen(s2);
	dest = ft_malloc((s1_len + s2_len) * (sizeof(char) + 1), 69);
	i = 0;
	while (s1[i] && i < s1_len - 1)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i++] = '/';
	dest[i] = '\0';
	ft_strncat(dest, s2, s2_len + s1_len + 1);
	return (dest);
}

char	*filename(char *cmd, t_data *data)
{
	char *(path), *(c_path);
	char **(arr);
	int (i);
	struct stat b;
	i = -1;
	if (!ft_strchr_ex(cmd, '/'))
	{
		stat(cmd, &b);
		if (S_ISDIR(b.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(PERMISSION_DENIED);
		}
		if (!access(cmd, F_OK))
		{
			if (!access(cmd, X_OK))
				return (cmd);
			else
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				exit(PERMISSION_DENIED);
			}
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(COMMAND_NOT_FOUND);
		}
		return (cmd);
	}
	path = get_env_value("PATH", data);
	if (!path)
		return (cmd);
	arr = ft_split(path, ':');
	while (arr[++i])
	{
		c_path = ft_strjoin_ex(arr[i], cmd);
		stat(c_path, &b);
		if (!access(c_path, F_OK) && !S_ISDIR(b.st_mode))
		{
			if (!access(c_path, X_OK))
				return (c_path);
			else
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(c_path, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				exit(PERMISSION_DENIED);
			}
		}
	}
	return (cmd);
}

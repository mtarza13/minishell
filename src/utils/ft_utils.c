
#include "../../include/minishell.h"

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!((str[0] >= 'a' && str[0] <= 'z')
			|| (str[0] >= 'A' && str[0] <= 'Z') || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!((str[i] >= 'a' && str[i] <= 'z')
				|| (str[i] >= 'A' && str[i] <= 'Z')
				|| (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	i;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (free_s2 ? s2 : ft_strdup(s2));
	if (!s2)
		return (free_s1 ? s1 : ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = ft_malloc(len1 + len2 + 1, 69);
	i = -1;
	while (++i < len1)
		result[i] = s1[i];
	i = -1;
	while (++i < len2)
		result[len1 + i] = s2[i];
	result[len1 + len2] = '\0';
	if (free_s1)
		free(s1);
	if (free_s2)
		free(s2);
	return (result);
}

char	*append_char_dynamic(char *str, char c, int *len, int *capacity)
{
	if (*len >= *capacity - 1)
	{
		*capacity *= 2;
		str = realloc(str, *capacity);
		if (!str)
			return (NULL);
	}
	str[(*len)++] = c;
	str[*len] = '\0';
	return (str);
}
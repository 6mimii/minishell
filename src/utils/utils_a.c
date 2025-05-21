/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_a.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:22:03 by mohamed-dou       #+#    #+#             */
/*   Updated: 2025/05/21 17:46:03 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	classify_token(char *token)
{
	if (strcmp(token, "|") == 0 || strcmp(token, ">") == 0 || strcmp(token,
			"<") == 0)
		printf("Operator: %s\n", token);
	else if (token[0] == '-')
		printf("Argument: %s\n", token);
	else
		printf("Command: %s\n", token);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (unsigned char)c)
		return ((char *)s);
	return (NULL);
}

char	*my_strtok(char *str, const char *delim)
{
	static char	*next;
	char		*token;

	if (str)
		next = str;
	if (!next || !*next)
		return (NULL);
	while (*next && ft_strchr(delim, *next))
		next++;
	if (!*next)
		return (NULL);
	if (ft_strchr("|><", *next))
	{
		token = next;
		next++;
		return (token);
	}
	token = next;
	while (*next && !ft_strchr(delim, *next) && !ft_strchr("|><", *next))
		next++;
	if (*next)
	{
		*next = '\0';
		next++;
	}
	return (token);
}

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}

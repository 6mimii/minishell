/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_b.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:13:09 by mdoudi-b          #+#    #+#             */
/*   Updated: 2025/03/07 17:10:06 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	slen;
	char	*sub;
	size_t	i;

	slen = ft_strlen(s);
	if (slen < start)
		return (ft_strdup(""));
	if (slen < start + len)
		len = slen - start;
	sub = (char *)malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
size_t	ft_strlen(const char *s)
{
	size_t	i;
	i = 0;
	while (s[i])
		i++;
	return (i);
}
size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size)
{
	size_t	i;
	size_t	src_len;

	src_len = ft_strlen(src);
	i = 0;
	if (dst_size == 0)
		return (src_len);
	while (src[i] != '\0' && i < dst_size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}
char *ft_strdup(const char *s1)
{
	char	*str;
	int		i;
	i = 0;
	while(s1[i])
		i++;
	if (!s1)
		return (NULL);
	str = (char *)malloc(sizeof(char) * i);
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	return (str);
}
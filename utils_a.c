/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_a.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdoudi-b <mdoudi-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:22:03 by mohamed-dou       #+#    #+#             */
/*   Updated: 2025/02/04 16:02:30 by mdoudi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	
	while(s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void    classify_token(char *token)
{
    if (strcmp(token, "|") == 0 || strcmp(token, ">") == 0 || strcmp(token, "<") == 0)
        printf("Operator: %s\n", token);
    else if (token[0] == '-') {
        printf("Argument: %s\n", token);
    }
    else {
        printf("Command: %s\n", token);
    }
}

char *ft_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (unsigned char)c)
            return (char *)s;
        s++;
    }
    if (*s == (unsigned char)c) 
        return (char *)s;
    return NULL;
}

char *my_strtok(char *str, const char *delim)
{
    static char *next;
    char        *token;

    if (str)
        next = str;
    if (!next || !*next)
        return NULL;
    while (*next && ft_strchr(delim, *next))
        next++;
    if (!*next)
        return NULL;
    if (ft_strchr("|><", *next)) // manejar >> y <<, las "grep ls -l " y ' '
    {
        token = next;
        next++;
        return token;
    }
    token = next;
    while (*next && !ft_strchr(delim, *next) && !ft_strchr("|><", *next))
        next++;
    if (*next)
    {
        *next = '\0';
        next++;
    }
    return token;
}

/* int main() {
    char str[] = "ls -l || grep .c > output.txt";
    char *delim = " ";
    char *token = my_strtok(str, delim);
    
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = my_strtok(NULL, delim);
    }
    return 0;
} */


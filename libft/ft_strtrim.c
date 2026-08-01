/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:33:13 by hounejja          #+#    #+#             */
/*   Updated: 2024/11/10 10:30:41 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	search(char const *s2, char c)
{
	while (*s2 && c != *s2)
	{
		s2++;
	}
	return (c == *s2);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	i = 0;
	while (search(set, s1[i]))
	{
		i++;
	}
	if (i == len)
	{
		return (ft_strdup(""));
	}
	while (search(set, s1[len - 1]))
	{
		len--;
	}
	return (ft_substr(s1, i, len - i));
}

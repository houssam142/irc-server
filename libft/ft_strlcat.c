/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:51:52 by hounejja          #+#    #+#             */
/*   Updated: 2024/10/25 11:38:42 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	l_dest;
	size_t	l_src;
	size_t	l;

	i = 0;
	l_src = ft_strlen(src);
	if (!dest && size == 0)
		return (l_src);
	l_dest = ft_strlen(dest);
	l = l_dest;
	if (size <= l_dest)
		return (l_src + size);
	else
	{
		while (src[i] && l_dest < size - 1)
			dest[l_dest++] = src[i++];
		dest[l_dest] = '\0';
		return (l + l_src);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:39:40 by hounejja          #+#    #+#             */
/*   Updated: 2024/10/24 21:13:56 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest_str, const void *src_str, size_t numBytes)
{
	char		*dest;
	const char	*src;

	if (!src_str && !dest_str)
		return (NULL);
	dest = dest_str;
	src = src_str;
	if (dest > src)
	{
		while (numBytes--)
		{
			dest[numBytes] = src[numBytes];
		}
	}
	else
	{	
		ft_memcpy(dest, src, numBytes);
	}	
	return (dest);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:33:35 by hounejja          #+#    #+#             */
/*   Updated: 2024/10/24 11:56:00 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	const unsigned char	*t;
	size_t				i;

	t = str;
	i = 0;
	while (i < n)
	{
		if (t[i] == (unsigned char)c)
		{
			return ((void *)(t + i));
		}
		i++;
	}
	return (NULL);
}

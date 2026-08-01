/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:54:42 by hounejja          #+#    #+#             */
/*   Updated: 2024/10/23 17:19:30 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	size_t				i;
	unsigned const char	*str3;
	unsigned const char	*str4;

	str3 = str1;
	str4 = str2;
	i = 0;
	while (i < n)
	{
		if (str3[i] != str4[i])
		{
			return (str3[i] - str4[i]);
		}
		i++;
	}
	return (0);
}

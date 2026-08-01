/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:55:53 by hounejja          #+#    #+#             */
/*   Updated: 2024/10/22 14:58:59 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int search_str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == (char)search_str)
		{
			break ;
		}
		i++;
	}
	if (str[i] == (char)search_str)
	{
		return ((char *)str + i);
	}
	return (0);
}

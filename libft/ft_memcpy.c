/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:09:36 by hounejja          #+#    #+#             */
/*   Updated: 2024/10/25 21:10:50 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest_str, const void *src_str, size_t n)
{
	size_t		i;
	char		*dest1;
	const char	*src1;

	if (!dest_str && !src_str)
		return (NULL);
	dest1 = (char *)dest_str;
	src1 = (const char *)src_str;
	i = 0;
	while (i < n)
	{
		dest1[i] = src1[i];
		i++;
	}
	return (dest_str);
}
//int main() {
//    int arr[] = {1, 2, 3, 4, 5};
//    int *arr1 = malloc(4 * sizeof(int)); // Allocate memory for arr1

//    ft_memcpy(arr1, arr, 5 * sizeof(int)); // Copy 4 bytes (the first int)

//    for (long unsigned int i = 0; i < 4; i++) {
//        printf("%d\n", arr1[i]);
//    }

//    return 0;
//}

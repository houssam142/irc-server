/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 20:19:48 by hounejja          #+#    #+#             */
/*   Updated: 2024/10/25 20:34:15 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	op;

	op = n;
	if (op == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (op < 0)
	{
		write(fd, "-", 1);
		op *= -1;
	}
	if (op > 9)
	{
		ft_putnbr_fd(op / 10, fd);
		ft_putnbr_fd(op % 10, fd);
	}
	if (op >= 0 && op <= 9)
	{
		ft_putchar_fd(op + 48, fd);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hounejja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 14:01:19 by hounejja          #+#    #+#             */
/*   Updated: 2024/11/06 13:59:48 by hounejja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*new_lst;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		new_lst = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = new_lst;
	}
	*lst = 0;
}

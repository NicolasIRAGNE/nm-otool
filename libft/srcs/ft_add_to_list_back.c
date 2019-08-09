/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_to_list_back.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 11:34:32 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/09 15:36:08 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_add_to_list_back(t_list **list, void *content, size_t size)
{
	t_list *node;

	if (!(node = ft_lstnew(content, size)))
		return (1);
	else
		ft_lstpushback(list, node);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_add_sorted_no_doubles.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 17:22:40 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/17 14:07:00 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_tree_add_sorted_value_no_doubles(t_tree **tree,
			void *content, size_t size, long (*sort)(void *, void *))
{
	if (*tree == NULL)
	{
		if (!(*tree = ft_tree_new(content, size)))
			return (1);
		else
			return (0);
	}
	else
	{
		if (sort((*tree)->content, content) > 0)
			return (ft_tree_add_sorted_value_no_doubles(&((*tree)->left),
				content, size, sort));
		else if (sort((*tree)->content, content) < 0)
			return (ft_tree_add_sorted_value_no_doubles(&((*tree)->right),
				content, size, sort));
		else
			return (2);
	}
}

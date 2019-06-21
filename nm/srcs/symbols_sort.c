/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 21:49:39 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/20 21:49:39 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

long	cmp_symbol_alpha(void *s1, void *s2)
{
	t_symbol *symbol1;
	t_symbol *symbol2;
	char	*name1;
	char	*name2;

	symbol1 = (t_symbol *)s1;
	symbol2 = (t_symbol *)s2;
	name1 = get_symbol_name(symbol1);
	name2 = get_symbol_name(symbol2);
	return (ft_strcmp(name1, name2));
}

long	cmp_symbol_numerical(void *s1, void *s2)
{
	t_symbol *symbol1;
	t_symbol *symbol2;
	uint64_t	value1;
	uint64_t	value2;

	symbol1 = (t_symbol *)s1;
	symbol2 = (t_symbol *)s2;
	value1 = get_symbol_value(symbol1);
	value2 = get_symbol_value(symbol2);
	if (value1 == value2)
		return (cmp_symbol_alpha(s1, s2));
	else
		return (value1 - value2);
}

long	cmp_symbol_none(void *s1, void *s2)
{
	(void)s1;
	(void)s2;
	return (-1);
}

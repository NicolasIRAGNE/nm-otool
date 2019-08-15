/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 21:49:39 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:53:21 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

long	cmp_symbol_numerical2(void *s1, void *s2, void *env)
{
	t_symbol		*symbol1;
	t_symbol		*symbol2;
	uint64_t		value1;
	uint64_t		value2;
	t_browser		*browser;

	browser = (t_browser *)env;
	symbol1 = (t_symbol *)s1;
	symbol2 = (t_symbol *)s2;
	value1 = get_symbol_value(symbol1);
	value2 = get_symbol_value(symbol2);
	if (value1 == value2)
	{
		if ((uint32_t)get_symbol_index(symbol2) > (uint32_t)browser->strsize)
			return (-1);
		else if ((uint32_t)get_symbol_index(symbol1) >
			(uint32_t)browser->strsize)
			return (1);
		return (ft_strcmp(get_symbol_name(symbol1),
			get_symbol_name(symbol2)));
	}
	else
		return (value1 > value2 ? 1 : -1);
}

long	cmp_bad_index_numerical(t_symbol *symbol1, t_symbol *symbol2, int *ret)
{
	*ret = 1;
	if (!has_relevant_value(symbol1) && has_relevant_value(symbol2))
		return (-1);
	else if (has_relevant_value(symbol1) && !has_relevant_value(symbol2))
		return (1);
	else if (!has_relevant_value(symbol1) && !has_relevant_value(symbol2))
	{
		return (ft_strcmp(get_symbol_name(symbol1),
			get_symbol_name(symbol2)));
	}
	*ret = 0;
	return (0);
}

long	cmp_same_value_numerical(t_symbol *symbol1, t_symbol *symbol2)
{
	if (has_bad_index(symbol1) && !has_bad_index(symbol2))
		return (-1);
	else if (!has_bad_index(symbol1) && has_bad_index(symbol2))
		return (1);
	return (-ft_strcmp(get_symbol_name(symbol1), get_symbol_name(symbol2)));
}

long	cmp_symbol_numerical(void *s1, void *s2, void *env)
{
	t_symbol	*symbol1;
	t_symbol	*symbol2;
	uint64_t	value1;
	uint64_t	value2;
	t_browser	*browser;

	browser = (t_browser *)env;
	symbol1 = (t_symbol *)s1;
	symbol2 = (t_symbol *)s2;
	value1 = get_symbol_value(symbol1);
	value2 = get_symbol_value(symbol2);
	browser->returned = 0;
	if (!browser->has_bad_index)
		browser->cmp_ret = cmp_bad_index_numerical(symbol1, symbol2,
			&browser->returned);
	if (browser->returned)
		return (browser->cmp_ret);
	if (value1 == value2)
		return (cmp_same_value_numerical(symbol1, symbol2));
	else
		return (value1 > value2 ? 1 : -1);
}

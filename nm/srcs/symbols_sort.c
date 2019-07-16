/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 21:49:39 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/16 18:05:22 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

long	cmp_symbol_numerical2(void *s1, void *s2, void *env)
{
	t_symbol		*symbol1;
	t_symbol		*symbol2;
	uint64_t		value1;
	uint64_t		value2;
	t_nm_browser	*browser;

	browser = (t_nm_browser *)env;
	symbol1 = (t_symbol *)s1;
	symbol2 = (t_symbol *)s2;
	value1 = get_symbol_value(symbol1);
	value2 = get_symbol_value(symbol2);	
	if (value1 == value2)
	{
		if ((uint32_t)get_symbol_index(symbol2) > (uint32_t)browser->strsize)
			return (-1);
		else if ((uint32_t)get_symbol_index(symbol1) > (uint32_t)browser->strsize)
			return (1);
		return (ft_strcmp(get_symbol_name(symbol1),
			get_symbol_name(symbol2)));
	}
	else
		return (value1 > value2 ? 1 : -1);
}

long	cmp_symbol_alpha(void *s1, void *s2, void *env)
{
	t_symbol *symbol1;
	t_symbol *symbol2;
	char	*name1;
	char	*name2;
//	int		ret;

	(void)env;
	symbol1 = (t_symbol *)s1;
	symbol2 = (t_symbol *)s2;
	name1 = get_symbol_name(symbol1);
	name2 = get_symbol_name(symbol2);
//	if (!(ret = (ft_strcmp(name1, name2))))
//	{
//		return (cmp_symbol_numerical2(s1, s2, env));
//	}
//	else(
//	)
	return (ft_strcmp(name1, name2));
//		return (ret);
}

long	cmp_symbol_numerical(void *s1, void *s2, void *env)
{
	t_symbol *symbol1;
	t_symbol *symbol2;
	uint64_t	value1;
	uint64_t	value2;
	t_nm_browser	*browser;

	browser = (t_nm_browser *)env;
	symbol1 = (t_symbol *)s1;
	symbol2 = (t_symbol *)s2;
	value1 = get_symbol_value(symbol1);
	value2 = get_symbol_value(symbol2);
	if (!browser->has_bad_index)
	{
		if (!has_relevant_value(symbol1) && has_relevant_value(symbol2))
			return (-1);
		else if (has_relevant_value(symbol1) && !has_relevant_value(symbol2))
			return (1);
		else if (!has_relevant_value(symbol1) && !has_relevant_value(symbol2))
			return (ft_strcmp(get_symbol_name(symbol1), get_symbol_name(symbol2)));
	}
	if (value1 == value2)
	{
		if (has_bad_index(symbol1) && !has_bad_index(symbol2))
			return (-1);
		else if (!has_bad_index(symbol1) && has_bad_index(symbol2))
			return (1);
		return (-ft_strcmp(get_symbol_name(symbol1), get_symbol_name(symbol2)));
	}
	else
	{
//		ft_printf("%lld - %lld = %lld\n", value1, value2 , value1 - value2);
//		ft_printf("%llx - %llx = %llx\n", value1, value2 , value1 - value2);
		return (value1 > value2 ? 1 : -1);
//		return ((long)value1 - (long)value2);
	}
}

long	cmp_symbol_none(void *s1, void *s2, void *env)
{
	(void)s1;
	(void)s2;
	(void)env;
	return (-1);
}

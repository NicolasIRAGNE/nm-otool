/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 17:45:01 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/20 18:40:16 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	*get_symbol_name(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
				? symbol->symbol_union.symbol32.name
					: symbol->symbol_union.symbol64.name);
}

uint64_t get_symbol_value(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
				? symbol->symbol_union.symbol32.nlist->n_value
					: symbol->symbol_union.symbol64.nlist->n_value);

}

int		is_same_name_symbol(void *s, void *str)
{
	t_symbol	*symbol;
	char		*name;

	symbol = (t_symbol *)s;
	name = (char *)str;
	return (ft_strcmp(get_symbol_name(symbol), name));
}

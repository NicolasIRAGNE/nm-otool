/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 17:45:01 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/16 17:54:02 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		 has_bad_index(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
			? symbol->symbol_union.symbol32.bad_index
				: symbol->symbol_union.symbol64.bad_index);
}

char	*get_symbol32_name(t_symbol32 *symbol32)
{
	return (symbol32->name);
}


char	*get_symbol64_name(t_symbol64 *symbol64)
{
	return (symbol64->name);
}

char	*get_symbol_name(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
				? get_symbol32_name(&symbol->symbol_union.symbol32)
					: get_symbol64_name(&symbol->symbol_union.symbol64));
}

uint64_t get_symbol_value(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
				? symbol->symbol_union.symbol32.nlist->n_value
					: symbol->symbol_union.symbol64.nlist->n_value);
}

int		get_symbol_index(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
		? symbol->symbol_union.symbol32.index
			: symbol->symbol_union.symbol64.index);
}

int		has_relevant_value(t_symbol *symbol)
{
	return ((symbol->symbol_enum == E_SYMBOL_32
		? (symbol->symbol_union.symbol32.nlist->n_type & N_TYPE) != N_UNDF || symbol->symbol_union.symbol32.bad_index
			: (symbol->symbol_union.symbol64.nlist->n_type & N_TYPE) != N_UNDF || symbol->symbol_union.symbol64.bad_index));
}

int		is_same_name_symbol(void *s, void *str)
{
	t_symbol	*symbol;
	char		*name;

	symbol = (t_symbol *)s;
	name = (char *)str;
	return (ft_strcmp(get_symbol_name(symbol), name));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 17:45:01 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:46:06 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int			has_bad_index(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
			? symbol->symbol_union.symbol32.bad_index
				: symbol->symbol_union.symbol64.bad_index);
}

char		*get_symbol_name(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
				? symbol->symbol_union.symbol32.name
					: symbol->symbol_union.symbol64.name);
}

uint64_t	get_symbol_value(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
				? symbol->symbol_union.symbol32.nlist->n_value
					: symbol->symbol_union.symbol64.nlist->n_value);
}

int			get_symbol_index(t_symbol *symbol)
{
	return (symbol->symbol_enum == E_SYMBOL_32
		? symbol->symbol_union.symbol32.index
			: symbol->symbol_union.symbol64.index);
}

int			has_relevant_value(t_symbol *symbol)
{
	return ((symbol->symbol_enum == E_SYMBOL_32
		? (symbol->symbol_union.symbol32.nlist->n_type & N_TYPE) != N_UNDF
		|| symbol->symbol_union.symbol32.bad_index
			: (symbol->symbol_union.symbol64.nlist->n_type & N_TYPE) != N_UNDF
			|| symbol->symbol_union.symbol64.bad_index));
}

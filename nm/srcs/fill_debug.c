/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_debug.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 01:46:28 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/22 01:46:28 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
** if the symbol is global (ie (n_type & N_EXT)) then nm 
** shall print an uppercase symbol, else its lowercase counterpart
*/
char	global_case_symbol(uint8_t n_type, char c)
{
	if (!(n_type & N_EXT))
		return (ft_tolower(c));
	else
		return (c);
}

int		process_fill_debug_from_section(t_symbol *symbol,
			uint8_t n_type, char *sectname)
{
	if (!ft_strcmp(sectname, SECT_TEXT))
		symbol->debug = global_case_symbol(n_type, 'T');
	else if (!ft_strcmp(sectname, SECT_DATA))
		symbol->debug = global_case_symbol(n_type, 'D');
	else if (!ft_strcmp(sectname, SECT_BSS))
		symbol->debug = global_case_symbol(n_type, 'B');
	else if (!ft_strcmp(sectname, SECT_COMMON))
		symbol->debug = global_case_symbol(n_type, 'S');
	else
		symbol->debug = global_case_symbol(n_type, 'S');
	return (0);
}

int		fill_debug_from_type(t_symbol *symbol, uint64_t type)
{
	if (type == N_UNDF)
		symbol->debug = 'U';
	else if (type == N_ABS)
		symbol->debug = 'A';
	else if (type == N_INDR)
		symbol->debug = 'I';
	else if (type == N_PBUD)
		symbol->debug = 'u';
	else
		symbol->debug = '?';
	return (0);
}

/*
** attribute a debug character for that symbol by peeking
** at its corresponding section
*/

int		fill_debug64(t_symbol *symbol, t_section_arr section_arr)
{
	int			nsect;
	t_symbol64	*symbol64;
	uint64_t	type;

	symbol64 = &symbol->symbol_union.symbol64;
	nsect = symbol64->nlist->n_sect;
	type = symbol64->nlist->n_type & N_TYPE;
	if (type == N_SECT)
	{
		if (symbol64->nlist->n_sect <= section_arr.size)
		{
			process_fill_debug_from_section(symbol, symbol64->nlist->n_type,
				section_arr.sections[symbol64->nlist->n_sect].
					section_union.section64->sectname);
			return (0);
		}
		else
		{
			//error corrupted
			return (1);
		}
	}
	else
		return (fill_debug_from_type(symbol, type));
}

int		fill_debug32(t_symbol *symbol, t_section_arr section_arr)
{
	int			nsect;
	t_symbol32	*symbol32;
	uint64_t	type;

	symbol32 = &symbol->symbol_union.symbol32;
	nsect = symbol32->nlist->n_sect;
	type = symbol32->nlist->n_type & N_TYPE;
	if (type == N_SECT)
	{
		if (symbol32->nlist->n_sect <= section_arr.size)
		{
			process_fill_debug_from_section(symbol, symbol32->nlist->n_type,
				section_arr.sections[symbol32->nlist->n_sect].
					section_union.section32->sectname);
			return (0);
		}
		else
		{
			//error corrupted
			return (1);
		}
	}
	else
		return (fill_debug_from_type(symbol, type));
}
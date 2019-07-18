/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_debug.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 01:46:28 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/17 17:51:57 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

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
//	printf("%s\n", get_symbol_name(symbol));
//	ft_printf("%s\n", sectname);
	if (!sectname)
		symbol->debug = global_case_symbol(n_type, 'S');
	else if (!ft_strcmp(sectname, SECT_TEXT))
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

int		fill_debug_from_type(t_symbol *symbol, uint8_t type, uint64_t value,
			t_browser *browser)
{
	if (type == N_UNDF && value != 0)// && !browser->has_bad_index)
		symbol->debug = 'U';
	else if ((type & N_TYPE) == N_ABS)
		symbol->debug = global_case_symbol(type, 'A');
	else if ((type & N_TYPE) == N_INDR)
		symbol->debug = global_case_symbol(type, 'I');
	else if ((type & N_TYPE) == N_PBUD && !browser->has_bad_index)
		symbol->debug = 'u';
	else
	{
		if (type & N_EXT && ((type & N_TYPE) == N_UNDF))
			symbol->debug = 'C';
		else
			symbol->debug = '?';
	}
	return (0);
}

/*
** attribute a debug character for that symbol by peeking
** at its corresponding section
*/

int		fill_debug64(t_symbol *symbol, t_section_arr section_arr,
			t_browser *browser)
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
		return (fill_debug_from_type(symbol, symbol64->nlist->n_type,
			symbol64->nlist->n_value, browser));
}

int		fill_debug32(t_symbol *symbol, t_section_arr section_arr,
			t_browser *browser)
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
		return (fill_debug_from_type(symbol, symbol32->nlist->n_type, symbol32->nlist->n_value, browser));
}

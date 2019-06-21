/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:29:27 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/21 01:29:27 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
** TODO NIRAGNE C'EST CETTE FONCTION LA OLALALALALALALALALALLA LALALALALALALA LALLALA LAL AL LAALLALALA
** cf page 452 of loader.h
** attributes a character for a given section flag (EN VRAI je sais pas si ca suffit pour mais je pense parce que faire des
** strcmp sur des section->sectname je pense c'est pas la bonne facon de faire apres ptet que ca l'est 😂)
*/

int		process_fill_debug(t_symbol *symbol, int section_flags)
{
	(void)section_flags;
	symbol->debug = 'X';
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

	symbol64 = &symbol->symbol_union.symbol64;
	nsect = symbol64->nlist->n_sect;
	if (symbol64->nlist->n_type & N_SECT)
	{
		if (symbol64->nlist->n_sect <= section_arr.size)
		{
			if (process_fill_debug(symbol, section_arr.
				sections[symbol64->nlist->n_sect].section_union.section64->flags))
				return (1);
		}
		else
		{
			//error corrupted
			return (1);
		}
	}
	else
		symbol->debug = 'U';
	return (0);
}

/*
** create and append a new symbol to the symbol tree as it is sorted by the
** appropriate function
**
** also fill its debug character by peeking at its corresponding section
*/
int		process_fill_symbols64(t_nm_browser *browser,
			int nsyms, int symoff, int stroff)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	t_symbol		*new_symbol;

	array = (void *)(browser->ptr + symoff);
	stringtable = (void *)(browser->ptr + stroff);
	i = 0;
	while (i < nsyms)
	{
		if (!(new_symbol = nm_new_symbol64(&array[i],
			stringtable + array[i].n_un.n_strx)))
			return (1);
		if (fill_debug64(new_symbol, browser->section_arr))
		{
			free(new_symbol);
			return (0);
		}
		if (add_symbol_to_browser(browser, new_symbol))
		{
			free(new_symbol);
			return (1);
		}
		i++;
	}
	return (0);
}

int		fill_symbol_table64(t_nm_browser *browser)
{
	struct mach_header_64 *header;
	struct load_command *lc;
	struct symtab_command *sym;
	uint64_t i;

	header = browser->header_union.header64;
	i = (uint64_t)(browser->ptr + sizeof(*header));
	while (i < (uint64_t)(browser->ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			if (process_fill_symbols64(browser, sym->nsyms,
				sym->symoff, sym->stroff))
				return (1);
		}
		i += lc->cmdsize;
	}
	return (0);
}

int		get_total_sections64(t_list *segments64)
{
	t_list						*ptr;
	struct segment_command_64	*seg;
	int							total;

	total = 0;
	ptr = segments64;
	while (ptr != NULL)
	{
		seg = (struct segment_command_64 *)ptr->content;
		total += seg->nsects;
		ptr = ptr->next;
	}
	return (total);
}

int		fill_sections_from_segment64(t_section *sections, int *index,
			struct segment_command_64 *seg)
{
	uint32_t i;

	i = 0;
	while (i < seg->nsects)
	{
		sections[*index].section_enum = E_SECTION_64;
		sections[(*index)++].section_union.section64 = (void *)seg + 
			sizeof(*seg) + i++ * sizeof(struct section_64);
	}
	return (0);
}

/*
** map all sections into a single array starting at index 1
*/
int		process_sections_array64(t_nm_browser *browser, t_list **segments)
{
	int							len;
	int							i;
	struct segment_command_64	*seg;

	len = get_total_sections64(*segments);
	browser->section_arr.size = len; // (len + 1) ?
	if (!(browser->section_arr.sections = malloc(sizeof(t_section) * (len + 1))))
	{
		ft_lstdel_ptr(segments);
		return (1);
	}
	else
	{
		browser->section_arr.sections[0].section_enum = E_SECTION_64;
		browser->section_arr.sections[0].section_union.section64 = NULL;
		i = 1;
		while (*segments != NULL)
		{
			seg = (struct segment_command_64 *)ft_lstpop_ptr(segments);
			fill_sections_from_segment64(browser->section_arr.sections, &i, seg);
		}
	}
	return (0);
}

/*
** gathers all segments in order to iterate over them and create an array for
** every sections
*/
int		get_sections64(t_nm_browser *browser)
{
	struct mach_header_64		*header;
	struct load_command			*lc;
	struct segment_command_64	*seg;
	uint64_t					i;
	t_list						*segments;

	segments = NULL;
	header = browser->header_union.header64;
	i = (uint64_t)(browser->ptr + sizeof(*header));
	while (i < (uint64_t)(browser->ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			if (ft_add_to_list_ptr_back(&segments, seg, sizeof(seg)))
				return (1);
		}
		i += lc->cmdsize;
	}
	return (process_sections_array64(browser, &segments));
}

/*
** store sections by index of apparition and symbol tables sorted 
** in a tree structure (stocks the debug Character (T, U, ...) by peeking
** at the matching section)
*/

int		fill_browser64(t_nm_browser *browser)
{
	if (get_sections64(browser))
		return (1);
	if (fill_symbol_table64(browser))
		return (1);
	return (0);
}

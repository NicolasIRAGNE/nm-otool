/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:29:27 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/16 17:46:23 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		overlaps_symtab_command(struct symtab_command *sym)
{
	uint32_t symbol_table_size;

	symbol_table_size = sym->nsyms * sizeof(struct nlist_64);
	if ((sym->symoff + symbol_table_size > sym->stroff
		&& sym->symoff < sym->stroff)
			|| (sym->stroff + sym->strsize > symbol_table_size
				&& sym->stroff < sym->symoff))
		return (1);
	else
		return (0);
}

/*
** create and append a new symbol to the symbol tree as it is sorted by the
** appropriate function
**
** also fill its debug character by peeking at its corresponding section
*/
int		process_fill_symbols64(t_header_parser *parser, t_nm_browser *browser,
			struct symtab_command *sym)
{
	uint32_t				i;
	char					*stringtable;
	struct nlist_64			*array;
	t_symbol				*new_symbol;
	struct mach_header_64	*header;

	header = parser->header_union.header64;
	array = (void *)((void *)header + sym->symoff);
	stringtable = (void *)((void *)header + sym->stroff);
	browser->strsize = sym->strsize;
	if (is_corrupted_data(stringtable, sym->strsize, browser))
		return (CORRUPTED);
	if (overlaps_symtab_command(sym))
	{
		ft_dprintf(2, "%s: %s truncated or malformed object (string table at"
		"offset %d with a size of %d, overlaps symbol table at offset %d with "
			"a size of %d)\n\n", browser->progname, browser->filename,
				sym->stroff, sym->strsize, sym->symoff,
					sym->nsyms * sizeof(struct nlist_64));
		return (CORRUPTED);
	}
	i = 0;
	while (i < sym->nsyms)
	{
		if (should_add_symbol(array[i].n_type, array[i].n_desc,
				stringtable + array[i].n_un.n_strx, browser))
		{
			if (!(new_symbol = nm_new_symbol64(&array[i],
				stringtable + array[i].n_un.n_strx,
					array[i].n_un.n_strx, browser)))
				return (1);
			if (fill_debug64(new_symbol, parser->section_arr, browser))
			{
				free(new_symbol);
				return (0);
			}
			if (add_symbol_to_browser(parser, browser, new_symbol))
			{
				free(new_symbol);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int		fill_symbol_table64(t_header_parser *parser, t_nm_browser *browser)
{
	struct load_command *lc;
	struct symtab_command *sym;
	uint64_t i;
	struct mach_header_64 *header;

	header = parser->header_union.header64;
	i = (uint64_t)((void *)header + sizeof(*header));
	while (i < (uint64_t)((void *)header + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			if (process_fill_symbols64(parser, browser, sym))
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
		sections[*index].section_union.section64 = (void *)seg + 
			sizeof(*seg) + i++ * sizeof(struct section_64);
		//swap_here
		(*index)++;
	}
	return (0);
}

/*
** map all sections into a single array starting at index 1 from a linked list
*/
int		process_sections_array64(t_header_parser *parser, t_list **segments)
{
	int							len;
	int							i;
	struct segment_command_64	*seg;

	len = get_total_sections64(*segments);
	parser->section_arr.size = len;
	if (!(parser->section_arr.sections = malloc(sizeof(t_section) * (len + 1))))
	{
		ft_lstdel_ptr(segments);
		return (1);
	}
	else
	{
		parser->section_arr.sections[0].section_enum = E_SECTION_64;
		parser->section_arr.sections[0].section_union.section64 = NULL;
		i = 1;
		while (*segments != NULL)
		{
			seg = (struct segment_command_64 *)ft_lstpop_ptr(segments);
			fill_sections_from_segment64(parser->section_arr.sections, &i, seg);
		}
	}
	return (0);
}

/*
** gathers all segments in a linked list in order to iterate over them 
** and create an array for every sections
*/
int		get_sections64(t_header_parser *parser, t_nm_browser *browser)
{
	struct load_command			*lc;
	struct segment_command_64	*seg;
	uint64_t					i;
	t_list						*segments;
	struct mach_header_64		*header;
	uint32_t					j;

	j = 0;
	header = parser->header_union.header64;
	segments = NULL;
	i = (uint64_t)((void *)header + sizeof(*header));
	while (j < header->ncmds)
	{
		lc = (struct load_command*) i;
		swap_load_command(lc, parser->should_swap);
		if (is_corrupted_data(lc, lc->cmdsize, browser)
			|| (void *)lc + max_uint32(lc->cmdsize, 1) > (void *)header + sizeof(*header) + header->sizeofcmds)
		{
			ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d extends past the end of all load commands in the file)\n\n",
					browser->progname, browser->filename, j);
			return (CORRUPTED);
		}
		if (lc->cmdsize % 8)
		{
			ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d fileoff not a multiple of 8)\n\n",
					browser->progname, browser->filename, j);
			return (CORRUPTED);
		}
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			if (is_corrupted_data(seg, sizeof(struct segment_command_64), browser))
			{
				return (CORRUPTED);
			}
			swap_segment_command_64(seg, parser->should_swap);
			
			if (seg->cmdsize != sizeof(*seg) + sizeof(struct section_64) * seg->nsects)
			{
				ft_dprintf(2, "%s: %s truncated or malformed object "
					"(load command %d inconsistent cmdsize in LC_SEGMENT_64 for the number of sections)\n\n",
						browser->progname, browser->filename, j);
				return (CORRUPTED);
			}
			if (is_corrupted_offset(parser->offset + seg->fileoff,
				seg->filesize, browser))
			{
				ft_dprintf(2, "%s: %s truncated or malformed object "
					"(load command %d fileoff filed plus filesize "
						"field in LC_SEGMENT_64 extends past the end of the"
					  		 "file)\n\n", browser->progname, browser->filename, j);
				return (CORRUPTED);
			}
			if (ft_add_to_list_ptr_back(&segments, seg, sizeof(seg)))
				return (1);
		}
		j++;
		i += lc->cmdsize;
	}
	return (process_sections_array64(parser, &segments));
}

/*
** store sections by index of apparition and symbol tables sorted 
** in a tree structure (stocks the debug Character (T, U, ...) by peeking
** at the matching section (mainly)
*/

int		fill_browser64(t_header_parser *parser, t_nm_browser *browser)
{
	int ret;

	if ((ret = get_sections64(parser, browser)))
		return (ret);
	if ((ret = fill_symbol_table64(parser, browser)))
		return (ret);
	if (ft_add_to_list_back(&browser->parsers, parser,
		sizeof(t_header_parser)))
	{
		return (1);
	}
	return (0);
}

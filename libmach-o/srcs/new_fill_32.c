/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 21:05:50 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 21:05:50 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		overlaps_symtab_command32(struct symtab_command *sym)
{
	uint32_t symbol_table_size;

	symbol_table_size = sym->nsyms * sizeof(struct nlist);
	if ((sym->symoff + symbol_table_size > sym->stroff
		&& sym->symoff < sym->stroff)
			|| (sym->stroff + sym->strsize > symbol_table_size
				&& sym->stroff < sym->symoff))
		return (1);
	else
		return (0);
}

int		is_corrupted_fill_symbol32(struct symtab_command *sym,
			t_browser *browser)
{
	char					*stringtable;

	stringtable = (char *)((void *)browser->ptr + sym->stroff);
	if (is_corrupted_data(stringtable, sym->strsize, browser))
	{
		ft_dprintf(2, "%s: %s: stringtable size (%u) exceeds end of file\n",
			browser->progname, browser->filename, sym->strsize);
		return (ERROR_CORRUPTED);
	}
	if (overlaps_symtab_command32(sym))
	{
		ft_dprintf(2, "%s: %s truncated or malformed object (string table at "
			"offset %d with a size of %d, overlaps symbol table at offset %d "
				"with a size of %d)\n\n", browser->progname, browser->filename,
					sym->stroff, sym->strsize, sym->symoff,
						sym->nsyms * sizeof(struct nlist));
		return (1);
	}
	return (0);
}

int		process_process_fill_symbol32(struct nlist *nlist,
			char *stringtable, t_header_parser *parser, t_browser *browser)
{
	t_symbol				*new_symbol;

	if (should_add_symbol(nlist->n_type, nlist->n_desc,
		stringtable + nlist->n_un.n_strx, browser))
	{
		if (!(new_symbol = nm_new_symbol32(nlist,
			stringtable + nlist->n_un.n_strx,
				nlist->n_un.n_strx, browser)))
			return (1);
		if (fill_debug32(new_symbol, parser->section_arr, browser))
		{
			free_symbol(new_symbol);
			return (2);
		}
		if (add_symbol_to_browser(parser, browser, new_symbol))
		{
			free_symbol(new_symbol);
			return (1);
		}
	}
	return (0);
}

int		process_process_fill_symbols32(struct symtab_command *sym,
		struct nlist *array, t_header_parser *parser, t_browser *browser)
{
	uint32_t				i;
	char					*stringtable;
	int						ret;

	stringtable = (void *)((void *)parser->header_union.header32 + sym->stroff);
	i = 0;
	while (i < sym->nsyms)
	{
		if ((ret = process_process_fill_symbol32(&array[i],
			stringtable, parser, browser)))
			return (ret == 1);
		i++;
	}
	return (0);
}

/*
** create and append a new symbol to the symbol tree as it is sorted by the
** appropriate function
**
** also fill its debug character by peeking at its corresponding section
*/

int		process_fill_symbols32(t_header_parser *parser, t_browser *browser,
			struct symtab_command *sym)
{
	struct nlist			*array;
	struct mach_header	*header;
	int						ret;

	header = parser->header_union.header32;
	array = (void *)((void *)header + sym->symoff);
	browser->strsize = sym->strsize;
	if ((ret = is_corrupted_fill_symbol32(sym, browser)))
		return (ret);
	return (process_process_fill_symbols32(sym, array, parser, browser));
}

int		fill_symbol_table32(t_header_parser *parser, t_browser *browser)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	uint64_t				i;
	struct mach_header	*header;
	int						ret;

	header = parser->header_union.header32;
	i = (uint64_t)((void *)header + sizeof(*header));
	while (i < (uint64_t)((void *)header + header->sizeofcmds))
	{
		lc = (struct load_command*)i;
		if (lc->cmdsize == 0)
			return (macho_perror(ERROR_CORRUPTED, browser));
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			if ((ret = process_fill_symbols32(parser, browser, sym)))
				return (ret);
		}
		i += lc->cmdsize;
	}
	return (0);
}

int		get_total_sections32(t_list *segments32)
{
	t_list						*ptr;
	struct segment_command		*seg;
	int							total;

	total = 0;
	ptr = segments32;
	while (ptr != NULL)
	{
		seg = (struct segment_command *)ptr->content;
		total += seg->nsects;
		ptr = ptr->next;
	}
	return (total);
}

int		fill_sections_from_segment32(t_section *sections, int *index,
			struct segment_command *seg, t_header_parser *parser)
{
	uint32_t i;

	i = 0;
	while (i < seg->nsects)
	{
		sections[*index].section_enum = E_SECTION_32;
		sections[*index].section_union.section32 = (void *)seg +
			sizeof(*seg) + i++ * sizeof(struct section);
		if (!ft_strcmp(sections[*index].section_union.section32->sectname,
				SECT_TEXT) && !ft_strcmp(sections[*index].
				section_union.section32->segname, SEG_TEXT))
			parser->text_section = &sections[*index];
		else if (!ft_strcmp(sections[*index].section_union.section32->sectname,
				SECT_DATA) && !ft_strcmp(sections[*index].
				section_union.section32->segname, SEG_DATA))
			parser->data_section = &sections[*index];
		swap_section(sections[*index].section_union.section32,
		parser->should_swap);
		(*index)++;
	}
	return (0);
}

/*
** map all sections into a single array starting at index 1 from a linked list
*/

int		process_sections_array32(t_header_parser *parser, t_list **segments)
{
	int							len;
	int							i;
	struct segment_command		*seg;

	len = get_total_sections32(*segments);
	parser->section_arr.size = len;
	if (!(parser->section_arr.sections = malloc(sizeof(t_section) * (len + 1))))
	{
		ft_lstdel_ptr(segments);
		return (1);
	}
	else
	{
		parser->section_arr.sections[0].section_enum = E_SECTION_32;
		parser->section_arr.sections[0].section_union.section32 = NULL;
		i = 1;
		while (*segments != NULL)
		{
			seg = (struct segment_command *)ft_lstpop_ptr(segments);
			fill_sections_from_segment32(parser->section_arr.sections, &i,
				seg, parser);
		}
	}
	return (0);
}

int		corrupted_load_command32(struct load_command *lc, t_browser *browser,
			struct mach_header *header, int index)
{
	if (lc->cmdsize == 0)
	{
		ft_dprintf(2, "%s: %s: truncated or malformed object (load commands"
				" extend past the end of the file)\n", browser->progname,
				browser->filename);
		return (1);
	}
	if (is_corrupted_data(lc, lc->cmdsize, browser)
			|| (void *)lc + max_uint32(lc->cmdsize, 1) > (void *)header
			+ sizeof(*header) + header->sizeofcmds)
	{
		ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d extends past the end of all"\
				"load commands in the file)\n\n",
				browser->progname, browser->filename, index);
		return (1);
	}
	if (lc->cmdsize % 4)
	{
		ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d fileoff not a multiple of 4)\n\n",
				browser->progname, browser->filename, index);
		return (1);
	}
	return (0);
}

int		is_corrupted_segment_command_32(struct segment_command *seg,
		t_header_parser *parser, t_browser *browser, int index)
{
	if (is_corrupted_data(seg, sizeof(struct segment_command),
				browser))
		return (1);
	swap_segment_command(seg, parser->should_swap);
	if (seg->cmdsize != sizeof(*seg) + sizeof(struct section)
			* seg->nsects)
	{
		ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d inconsistent cmdsize in LC_SEGMENT "
				"for the number of sections)\n\n",
				browser->progname, browser->filename, index);
		return (1);
	}
	if (is_corrupted_offset(parser->offset + seg->fileoff,
				seg->filesize, browser))
	{
		ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d fileoff filed plus filesize "
				"field in LC_SEGMENT extends past the end of the "
				"file)\n\n", browser->progname, browser->filename, index);
		return (1);
	}
	return (0);
}

int		get_sections_load_command_32(t_browser *browser,
			t_header_parser *parser, struct load_command *lc, int index)
{
	struct mach_header		*header;
	struct segment_command	*seg;

	header = parser->header_union.header32;
	swap_load_command(lc, parser->should_swap);
	if (corrupted_load_command32(lc, browser, header, index))
		return (ERROR_CORRUPTED);
	if (lc->cmd == LC_SEGMENT)
	{
		seg = (struct segment_command *)lc;
		if (is_corrupted_segment_command_32(seg, parser, browser, index))
			return (ERROR_CORRUPTED);
		if (ft_add_to_list_ptr_back(browser->segments, seg, sizeof(seg)))
			return (1);
	}
	return (0);
}

int		get_sections_lcs_32(t_browser *browser, t_header_parser *parser,
			struct mach_header *header)
{
	struct load_command			*lc;
	uint32_t					j;
	uint64_t					i;
	int							ret;

	i = (uint64_t)((void *)header + sizeof(*header));
	j = 0;
	header = parser->header_union.header32;
	while (j < header->ncmds)
	{
		lc = (struct load_command *)i;
		if ((ret = get_sections_load_command_32(browser, parser, lc, j)))
			return (ret);
		j++;
		i += lc->cmdsize;
	}
	return (0);
}

/*
** gathers all segments in a linked list in order to iterate over them
** and create an array for every sections
*/

int		get_sections32(t_header_parser *parser, t_browser *browser)
{
	t_list						*segments;
	struct mach_header			*header;
	int							ret;

	header = parser->header_union.header32;
	parser->parser_union.arch.cputype = header->cputype;
	parser->parser_union.arch.cpusubtype = header->cpusubtype;
	segments = NULL;
	browser->segments = &segments;
	if ((ret = get_sections_lcs_32(browser, parser, header)))
		return (ft_lstdel_ptr_ret(&segments, ret));
	return (process_sections_array32(parser, &segments));
}

/*
** store sections by index of apparition and symbol tables sorted
** in a tree structure (stocks the debug Character (T, U, ...) by peeking
** at the matching section (mainly)
*/

int		fill_browser32(t_header_parser *parser, t_browser *browser)
{
	int ret;

	if ((ret = get_sections32(parser, browser)))
		return (ret);
	if ((ret = fill_symbol_table32(parser, browser)))
	{
		if (ret == ERROR_CORRUPTED)
		{
			if (add_parser(browser, parser))
			{
				free_parser(parser);
				return (1);
			}
		}
		else
			free_parser(parser);
		return (ret);
	}
	if (add_parser(browser, parser))
	{
		free_parser(parser);
		return (1);
	}
	return (0);
}

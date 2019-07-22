/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 02:25:02 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/22 16:48:50 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "mach_o.h"

/*
** create and append a new symbol to the symbol tree as it is sorted by the
** appropriate function
**
** also fill its debug character by peeking at its corresponding section
*/
int		process_fill_symbols32(t_header_parser *parser,
			t_browser *browser,
				struct symtab_command *sym)
{
	uint32_t			i;
	char				*stringtable;
	struct nlist		*array;
	t_symbol			*new_symbol;
	struct mach_header	*header;
	int					ret;

	header = parser->header_union.header32;
	array = (void *)header + sym->symoff;
	stringtable = (void *)header + sym->stroff;
	browser->strsize = sym->strsize;
	i = 0;
	while (i < sym->nsyms)
	{
		swap_nlist(&array[i], parser->should_swap);
		if (should_add_symbol(array[i].n_type, array[i].n_desc,
			stringtable + array[i].n_un.n_strx, browser))
		{
	//		ft_printf("%lld\n", array[i].n_type);
	//		ft_printf("%lld\n", array[i].n_desc);
	//		ft_printf("%s\n", stringtable + array[i].n_un.n_strx);
			if (!(new_symbol = nm_new_symbol32(&array[i],
				stringtable + array[i].n_un.n_strx,
					array[i].n_un.n_strx, browser)))
				return (1);
			if ((ret = fill_debug32(new_symbol, parser->section_arr, browser)))
			{
			//	if (browser->has_64)
			//	{
			//		free(new_symbol);
			//		return (ret == 2 ? 0 : 1);
			//	}
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

int		fill_symbol_table32(t_header_parser *parser, t_browser *browser)
{
	struct load_command 	*lc;
	struct symtab_command	*sym;
	uint64_t				i;
	struct mach_header		*header;

	header = parser->header_union.header32;
	i = (uint64_t)((void *)header + sizeof(*header));
	while (i < (uint64_t)((void *)header + header->sizeofcmds))
	{
		lc = (struct load_command*)i;
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			swap_symtab_command(sym, parser->should_swap);
			if (process_fill_symbols32(parser, browser, sym))
				return (1);
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
				SECT_TEXT)
				&& !ft_strcmp(sections[*index]
					.section_union.section32->segname, SEG_TEXT))
			parser->text_section = &sections[*index];
		swap_section(sections[*index].section_union.section32, parser->should_swap);
		(*index)++;
	}
	return (0);
}

/*
** map all sections into a single array starting at index 1, storing it in
** parser->section_arr.sections
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

/*
** gathers all segments in order to iterate over them and create an array for
** every sections
*/
int		get_sections32(t_header_parser *parser, t_browser *browser)
{
	struct mach_header			*header;
	struct load_command			*lc;
	struct segment_command		*seg;
	uint64_t					i;
	t_list						*segments;
	int							j;

	header = parser->header_union.header32;
	segments = NULL;
	i = (uint64_t)((void *)header + sizeof(*header));
	j = 0;
	while (j < (int)header->ncmds)
	{
		lc = (struct load_command*) i;
		swap_load_command(lc, parser->should_swap);
		if (is_corrupted_data(lc, lc->cmdsize, browser)
				|| (void *)lc + max_uint32(lc->cmdsize, 1) > (void *)header + sizeof(*header) + header->sizeofcmds)
		{
		//	nm_print(browser, 1);
			ft_dprintf(2, "%s: %s truncated or malformed object "
				"(load command %d extends past the end of all load commands in the file)\n\n",
					browser->progname, browser->filename, j);
			return (CORRUPTED);
		}
		if (lc->cmdsize % 4)
		{
			ft_dprintf(2, "%s: %s truncated or malformed object "
					"(load command %d fileoff not a multiple of 4)\n\n",
					browser->progname, browser->filename, j);
			return (CORRUPTED);
		}
		if (lc->cmd == LC_SEGMENT)
		{
			seg = (struct segment_command *)lc;
			if (is_corrupted_data(seg, sizeof(struct segment_command), browser))
			{
				return (0);
			}
			swap_segment_command(seg, parser->should_swap);
			if (is_corrupted_offset(parser->offset + seg->fileoff, seg->filesize, browser))
			{
			//	nm_print(browser, 1);
				ft_dprintf(2, "%s: %s truncated or malformed object "
					"(load command %d fileoff filed plus filesize "
						"field in LC_SEGMENT extends past the end of the"
							"file)\n", browser->progname, browser->filename, j);
				return (CORRUPTED);
			}
			if (ft_add_to_list_ptr_back(&segments, seg, sizeof(seg)))
				return (1);
		}
		i += lc->cmdsize;
		j++;
	}
	return (process_sections_array32(parser, &segments));
}

/*
** store sections by index of apparition and symbol tables sorted 
** in a tree structure (stocks the debug Character (T, U, ...) by peeking
** at the matching section)
*/

int		fill_browser32(t_header_parser *parser,
			t_browser *browser)
{
	int ret;

	if ((ret = get_sections32(parser, browser)))
		return (ret);
	if (browser->ret)
		return (0);
	if ((ret = fill_symbol_table32(parser, browser)))
		return (ret);
	if (browser->ret)
		return (0);
	if (add_parser(browser, parser))
		return (1);
	return (0);
}

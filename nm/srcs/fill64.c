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
** create and append a new symbol to the symbol tree as it is sorted by the
** appropriate function
**
** also fill its debug character by peeking at its corresponding section
*/
int		process_fill_symbols64(t_header_parser *parser, t_nm_browser *browser,
			struct symtab_command *sym)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	t_symbol		*new_symbol;
	struct mach_header_64 *header;

	header = parser->header_union.header64;
	array = (void *)((void *)header + sym->symoff);
	stringtable = (void *)((void *)header + sym->stroff);
	i = 0;
	while (i < sym->nsyms)
	{
		if (should_add_symbol(array[i].n_type, browser))
		{
			if (!(new_symbol = nm_new_symbol64(&array[i],
							stringtable + array[i].n_un.n_strx)))
				return (1);
			if (fill_debug64(new_symbol, parser->section_arr))
			{
				free(new_symbol);
				return (0);
			}
			if (add_symbol_to_browser(browser, new_symbol))
			{
				free(new_symbol);
				return (1);
			}
			browser->has_64 = 1;
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
		sections[(*index)++].section_union.section64 = (void *)seg + 
			sizeof(*seg) + i++ * sizeof(struct section_64);
	}
	return (0);
}

/*
** map all sections into a single array starting at index 1
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
** gathers all segments in order to iterate over them and create an array for
** every sections
*/
int		get_sections64(t_header_parser *parser)
{
	struct load_command			*lc;
	struct segment_command_64	*seg;
	uint64_t					i;
	t_list						*segments;
	struct mach_header_64 *header;

	header = parser->header_union.header64;
	segments = NULL;
	i = (uint64_t)((void *)header + sizeof(*header));
	while (i < (uint64_t)((void *)header + header->sizeofcmds))
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
	return (process_sections_array64(parser, &segments));
}

/*
** store sections by index of apparition and symbol tables sorted 
** in a tree structure (stocks the debug Character (T, U, ...) by peeking
** at the matching section)
*/

int		fill_browser64(t_header_parser *parser, t_nm_browser *browser)
{
	if (get_sections64(parser))
		return (1);
	if (fill_symbol_table64(parser, browser))
		return (1);
	return (0);
}

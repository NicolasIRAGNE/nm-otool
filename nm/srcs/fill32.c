/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 02:25:02 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/22 02:25:02 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ft_nm.h"

/*
** create and append a new symbol to the symbol tree as it is sorted by the
** appropriate function
**
** also fill its debug character by peeking at its corresponding section
*/
int		process_fill_symbols32(t_nm_browser *browser,
			int nsyms, int symoff, int stroff)
{
	int				i;
	char			*stringtable;
	struct nlist	*array;
	t_symbol		*new_symbol;

	array = (void *)(browser->ptr + symoff);
	stringtable = (void *)(browser->ptr + stroff);
	i = 0;
	while (i < nsyms)
	{
		if (should_add_symbol(array[i].n_type, browser))
		{
			if (!(new_symbol = nm_new_symbol32(&array[i],
							stringtable + array[i].n_un.n_strx)))
				return (1);
			if (fill_debug32(new_symbol, browser->section_arr))
			{
				free(new_symbol);
				return (0); //error
			}
			if (add_symbol_to_browser(browser, new_symbol))
			{
				free(new_symbol);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int		fill_symbol_table32(t_nm_browser *browser)
{
	struct mach_header *header;
	struct load_command *lc;
	struct symtab_command *sym;
	uint64_t i;

	header = browser->header_union.header32;
	i = (uint64_t)(browser->ptr + sizeof(*header));
	while (i < (uint64_t)(browser->ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			if (process_fill_symbols32(browser, sym->nsyms,
				sym->symoff, sym->stroff))
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
			struct segment_command *seg)
{
	uint32_t i;

	i = 0;
	while (i < seg->nsects)
	{
		sections[*index].section_enum = E_SECTION_32;
		sections[(*index)++].section_union.section32 = (void *)seg + 
			sizeof(*seg) + i++ * sizeof(struct section);
	}
	return (0);
}

/*
** map all sections into a single array starting at index 1
*/
int		process_sections_array32(t_nm_browser *browser, t_list **segments)
{
	int							len;
	int							i;
	struct segment_command		*seg;

	len = get_total_sections32(*segments);
	browser->section_arr.size = len;
	if (!(browser->section_arr.sections = malloc(sizeof(t_section) * (len + 1))))
	{
		ft_lstdel_ptr(segments);
		return (1);
	}
	else
	{
		browser->section_arr.sections[0].section_enum = E_SECTION_32;
		browser->section_arr.sections[0].section_union.section32 = NULL;
		i = 1;
		while (*segments != NULL)
		{
			seg = (struct segment_command *)ft_lstpop_ptr(segments);
			fill_sections_from_segment32(browser->section_arr.sections, &i, seg);
		}
	}
	return (0);
}

/*
** gathers all segments in order to iterate over them and create an array for
** every sections
*/
int		get_sections32(t_nm_browser *browser)
{
	struct mach_header			*header;
	struct load_command			*lc;
	struct segment_command		*seg;
	uint64_t					i;
	t_list						*segments;

	segments = NULL;
	header = browser->header_union.header32;
	i = (uint64_t)(browser->ptr + sizeof(*header));
	while (i < (uint64_t)(browser->ptr + header->sizeofcmds))
	{
		lc = (struct load_command*) i;
		if (lc->cmd == LC_SEGMENT)
		{
			seg = (struct segment_command *)lc;
			if (ft_add_to_list_ptr_back(&segments, seg, sizeof(seg)))
				return (1);
		}
		i += lc->cmdsize;
	}
	return (process_sections_array32(browser, &segments));
}

/*
** store sections by index of apparition and symbol tables sorted 
** in a tree structure (stocks the debug Character (T, U, ...) by peeking
** at the matching section)
*/

int		fill_browser32(t_nm_browser *browser)
{
	if (get_sections32(browser))
		return (1);
	if (fill_symbol_table32(browser))
		return (1);
	return (0);
}

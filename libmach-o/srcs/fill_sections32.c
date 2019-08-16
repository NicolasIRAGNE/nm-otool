/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_sections32.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 15:09:59 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 15:10:53 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

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

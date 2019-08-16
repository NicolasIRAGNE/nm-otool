/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_sections.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 20:54:40 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 15:39:55 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void	print_parser_header_intro(t_header_parser *parser)
{
	char *str;

	str = get_cpu_name(parser->parser_union.arch.cputype,
			parser->parser_union.arch.cpusubtype);
	if ((parser->parser_enum == PARSER_ENUM_NONE || !ft_strcmp(str, ARCH) ||
			!ft_strcmp("", str)) && parser->parser_enum != PARSER_ENUM_OBJECT)
		ft_printf("%s:\n", parser->filename);
	else if (parser->parser_enum == PARSER_ENUM_ARCHI
			&& parser->parser_union.arch.relevant)
	{
		ft_printf("%s (architecture %s):\n",
				parser->filename, get_cpu_name(parser->parser_union.
					arch.cputype, parser->parser_union.arch.cpusubtype));
	}
	else if (parser->parser_enum == PARSER_ENUM_OBJECT)
	{
		ft_printf("%s(%s):\n",
				parser->filename, parser->parser_union.object.name);
	}
}

void	otool_process_print_header_parser(t_header_parser *parser,
			cpu_type_t cputype, t_section *section)
{
	ft_printf("Contents of (%s,%s) section\n", get_section_segname(section),
		get_section_sectname(section));
	if (cputype == CPU_TYPE_I386 || cputype == CPU_TYPE_X86_64)
	{
		print_text_section((void *)(parser->ptr) + get_section_offset(section),
			get_section_size(section), get_section_addr(section),
				section->section_enum == E_SECTION_32 ? 8 : 16);
	}
	else
	{
		print_text_section_4_by_4(section, parser);
	}
}

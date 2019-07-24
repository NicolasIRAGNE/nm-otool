/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 13:24:40 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/24 13:15:42 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int is_little_endian(void)
{
	int num;
  
	num = 1;
	if (*(char *)&num == 1)
		return (1);
	else
		return (0);
}

void	print_parser_header_intro(t_header_parser *parser)
{
	char *str;

	str = get_cpu_name(parser->parser_union.arch.cputype,
			parser->parser_union.arch.cpusubtype);
	if (parser->parser_enum == PARSER_ENUM_NONE || !ft_strcmp(str, ARCH) ||
			!ft_strcmp("", str))
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

void    print_text_section(unsigned char *data, uint64_t data_size,
		uint64_t addr, int alignment)
{
	size_t          i;
	int				j;

	i = 0;
	while (i < data_size)
	{
		j = 0;
		ft_printf("%0*llx\t", alignment, addr + i);
		while (j < 16 && i + j < data_size)
		{
			ft_printf("%.2x ", data[i + j]);
			j++;
		}
		ft_printf("\n");
		i += 16;
	}
}

void	fill_text_printer(t_section *section, t_header_parser *parser, t_text_printer *printer)
{
	printer->alignment = section->section_enum == E_SECTION_32 ? 8 : 16;
	printer->addr = get_section_addr(section);
	printer->data = (void *)parser->ptr + get_section_offset(section);
	printer->data_size = get_section_size(section);
	printer->should_swap = (is_little_endian() != parser->should_swap);
}

void	print_text_section_4_by_4(t_section *section, t_header_parser *parser)
{
	size_t          i;
	int				j;
	int				k;
	t_text_printer	p;

	fill_text_printer(section, parser, &p);
	i = 0;
	while (i < p.data_size)
	{
		j = 0;
		ft_printf("%0*llx\t", p.alignment, p.addr + i);
		while (j < 16 && i + j < p.data_size)
		{
			k = 0;
			while (k < 4 && i + j + k < p.data_size)
			{
				if (p.should_swap)
					ft_printf("%.2x", p.data[i + j + 4 - k - 1]);
				else
					ft_printf("%.2x", p.data[i + j + k]);
				k++;
			}
			j += 4;
			ft_printf(" ");
		}
		ft_printf("\n");
		i += 16;
	}
}

void	otool_process_print_header_parser(t_header_parser *parser,
			cpu_type_t cputype, t_section *section)
{
	//ft_printf("CPU: %d\n", cputype);
	ft_printf("Contents of (%s,%s) section\n", get_section_segname(section),
		get_section_sectname(section));
	if (cputype == CPU_TYPE_I386 || cputype == CPU_TYPE_X86_64)
	{
		print_text_section((void *)(parser->ptr) + get_section_offset(section),
			get_section_size(section), get_section_addr(section),
				section->section_enum == E_SECTION_32 ? 8 : 16);
	}
	else if (0)
	{

	}
	else
	{
		print_text_section_4_by_4(section, parser);
	}
}

void	otool_print_header_parser(t_header_parser *parser,
			t_browser *browser, int len)
{
	t_section *section;

	(void)len;
	print_parser_header_intro(parser);
	if (browser->has_bad_index)
	{
		ft_dprintf(2, "%s: '%s': truncated or malformed object ("
			"bad string table index: %d past the end of"
				" string table, for symbol at index %d)\n", browser->progname, 
					browser->filename, browser->bad_string_index,
						browser->bad_symbol_index);
		return ;
	}
	else if ((section = parser->text_section))
	{
		otool_process_print_header_parser(parser,
			parser->parser_union.arch.cputype, section);
	}
}

void	process_otool_print_tree(t_browser *browser, t_tree *tree, int len)
{
	t_header_parser *parser;

	if (tree)
	{
		process_otool_print_tree(browser, tree->left, len);
		parser = (t_header_parser *)tree->content;
		otool_print_header_parser(parser, browser, len);
		process_otool_print_tree(browser, tree->right, len);
	}
}

void	otool_print(t_browser *browser)
{
	int len;

	len = ft_treelen(browser->parsers);
//	ft_printf("%s:\n", browser->filename);
	process_otool_print_tree(browser, browser->parsers, len);
}

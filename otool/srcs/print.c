/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 13:24:40 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/18 19:04:25 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

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
		while (j < alignment && i + j < data_size)
		{
			ft_printf("%.2x ", data[i + j]);
			j++;
		}
		ft_printf("\n");
		i += alignment;
	}
}

void	otool_print_header_parser(t_header_parser *parser, t_browser *browser, int len)
{
	t_section *section;

	(void)len;
	(void)browser;
	if ((section = parser->text_section))
	{
		ft_printf("Contents of (%s,%s) section\n", get_section_segname(section),
				get_section_sectname(section));
		print_text_section((void *)(parser->ptr) + get_section_offset(section),
			get_section_size(section), get_section_addr(section),
				section->section_enum == E_SECTION_32 ? 8 : 16);
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
	ft_printf("%s:\n", browser->filename);
	process_otool_print_tree(browser, browser->parsers, len);
}

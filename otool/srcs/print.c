/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 13:24:40 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 15:31:48 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

int		is_little_endian(void)
{
	int	num;

	num = 1;
	if (*(char *)&num == 1)
		return (1);
	else
		return (0);
}

void	process_otool_print_header_parser(t_header_parser *parser,
			t_otool_flags *flags)
{
	t_section *section;

	if (flags->flag_t && (section = parser->text_section))
	{
		otool_process_print_header_parser(parser,
				parser->parser_union.arch.cputype, section);
	}
	if (flags->flag_d && (section = parser->data_section))
	{
		otool_process_print_header_parser(parser,
				parser->parser_union.arch.cputype, section);
	}
}

void	otool_print_header_parser(t_header_parser *parser,
			t_browser *browser, int len, t_otool_flags *flags)
{
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
	process_otool_print_header_parser(parser, flags);
}

void	process_otool_print_tree(t_browser *browser, t_tree *tree, int len,
t_otool_flags *flags)
{
	t_header_parser *parser;

	if (tree)
	{
		process_otool_print_tree(browser, tree->left, len, flags);
		parser = (t_header_parser *)tree->content;
		otool_print_header_parser(parser, browser, len, flags);
		process_otool_print_tree(browser, tree->right, len, flags);
	}
}

void	otool_print(t_browser *browser, t_otool_flags *flags)
{
	int len;

	if (browser->from == E_ARCHIVE)
		ft_printf("Archive : %s\n", browser->filename);
	len = ft_treelen(browser->parsers);
	process_otool_print_tree(browser, browser->parsers, len, flags);
}

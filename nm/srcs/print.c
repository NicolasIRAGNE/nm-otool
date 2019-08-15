/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/08/15 16:42:43 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	get_debug(char debug, int has_bad_index)
{
	(void)has_bad_index;
	if (has_bad_index && (debug == 'U'))//debug != 'C' && debug != 'I' && debug != 'A'))
		return ('?');
	else
		return (debug);
}

void	print_parser_header_intro(t_header_parser *parser)
{
//	ft_printf("%d %d\n", parser->parser_enum, PARSER_ENUM_OBJECT);
//	ft_printf("%s\n", parser->filename);
//	ft_printf("%s\n", parser->parser_union.object.name);
	if (parser->parser_enum == PARSER_ENUM_NONE)
		ft_printf("\n%s:\n", parser->filename);
	else if (parser->parser_enum == PARSER_ENUM_ARCHI
			&& parser->parser_union.arch.relevant)
	{
		ft_printf("\n%s (for architecture %s):\n",
				parser->filename,
				get_cpu_name(parser->parser_union.arch.cputype,
					parser->parser_union.arch.cpusubtype));
	}
	else if (parser->parser_enum == PARSER_ENUM_OBJECT)
	{
		ft_printf("\n%s(%s):\n", parser->filename, parser->parser_union.object.name);
	}
}

void	print_symbol64(t_header_parser *parser, t_symbol64 symbol64,
			char debug, t_browser *browser)
{
	(void)parser;
	if ((!((symbol64.nlist->n_type & N_TYPE) == N_UNDF) ||
		browser->has_bad_index) ||
			(parser->parser_enum == PARSER_ENUM_OBJECT &&
			 	symbol64.nlist->n_value > 0))
	{
		ft_printf("%016llx %c %s\n", symbol64.nlist->n_value,
			get_debug(debug, browser->has_bad_index),
				symbol64.name);
	}
	else
	{
		ft_printf("%18s %*s\n", "U", (symbol64.length != -1 ? symbol64.length
			: ft_strlen(symbol64.name)),
				symbol64.name);
	}
}

void	print_symbol32(t_symbol32 symbol32, char debug, t_browser *browser)
{
	if (!((symbol32.nlist->n_type & N_TYPE) == N_UNDF)
			|| browser->has_bad_index)
	{
		ft_printf("%08llx %c %s\n", symbol32.nlist->n_value,
			get_debug(debug, browser->has_bad_index),
				symbol32.name);
	}
	else
	{
		ft_printf("%10s %s\n", "U", symbol32.name);
	}
}

void	print_symbol(t_header_parser *parser, t_symbol *symbol, t_browser *browser)
{
	if (symbol->symbol_enum == E_SYMBOL_64)
		print_symbol64(parser, symbol->symbol_union.symbol64, symbol->debug, browser);
	else
		print_symbol32(symbol->symbol_union.symbol32, symbol->debug, browser);
}

void	print_symbol_tree(t_header_parser *parser, t_tree *tree, t_browser *browser)
{
	if (tree)
	{
		print_symbol_tree(parser, tree->left, browser);
		print_symbol(parser, (t_symbol *)tree->content, browser);
		print_symbol_tree(parser, tree->right, browser);
	}
}

void	nm_print_header_parser(t_header_parser *parser,
			t_browser *browser, int len, int force)
{
	if (browser->nb_args > 1 || len > 1 || force)
		print_parser_header_intro(parser);
	print_symbol_tree(parser, parser->symbols, browser);
}

void	process_nm_print_tree(t_browser *browser, t_tree *tree, int len)
{
	t_header_parser *parser;

	if (tree)
	{
		process_nm_print_tree(browser, tree->left, len);
		parser = (t_header_parser *)tree->content;
		nm_print_header_parser(parser, browser, len, browser->force);
		process_nm_print_tree(browser, tree->right, len);
	}
}

void	nm_print(t_browser *browser, int force)
{
	int				len;

	len = ft_treelen(browser->parsers);
	if (force)
		browser->force = 1;
	process_nm_print_tree(browser, browser->parsers, len);
}

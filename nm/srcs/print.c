/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/07/16 17:51:42 by ldedier          ###   ########.fr       */
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
	if (parser->cputype == -1)
		ft_printf("\n%s:\n", parser->filename);
	else
		ft_printf("\n%s (for architecture %s):\n",
			parser->filename, get_cpu_name(parser->cputype, parser->cpusubtype));
}

void	print_symbol64(t_symbol64 symbol64, char debug, t_nm_browser *browser)
{
	if (!((symbol64.nlist->n_type & N_TYPE) == N_UNDF) || browser->has_bad_index)
	{
		ft_printf("%016llx %c %s\n", symbol64.nlist->n_value,
			get_debug(debug, browser->has_bad_index),
				get_symbol64_name(&symbol64));
	}
	else
	{
		ft_printf("%18s %*s\n", "U", (symbol64.length != -1 ? symbol64.length
			: ft_strlen(get_symbol64_name(&symbol64))),
				get_symbol64_name(&symbol64));
	}
}

void	print_symbol32(t_symbol32 symbol32, char debug, t_nm_browser *browser)
{
	if (!((symbol32.nlist->n_type & N_TYPE) == N_UNDF)
			|| browser->has_bad_index)
	{
		ft_printf("%08llx %c %s\n", symbol32.nlist->n_value,
			get_debug(debug, browser->has_bad_index),
				get_symbol32_name(&symbol32));
	}
	else
	{
		ft_printf("%10s %s\n", "U", get_symbol32_name(&symbol32));
	}
}

void	print_symbol(t_symbol *symbol, t_nm_browser *browser)
{
	if (symbol->symbol_enum == E_SYMBOL_64)
		print_symbol64(symbol->symbol_union.symbol64, symbol->debug, browser);
	else
		print_symbol32(symbol->symbol_union.symbol32, symbol->debug, browser);
}

void	print_symbol_tree(t_tree *tree, t_nm_browser *browser)
{
	if (tree)
	{
		print_symbol_tree(tree->left, browser);
		print_symbol((t_symbol *)tree->content, browser);
		print_symbol_tree(tree->right, browser);
	}
}

void	nm_print_header_parser(t_header_parser *parser,
			t_nm_browser *browser, int len, int force)
{
	if (browser->nb_args > 1 || len > 1 || force)
		print_parser_header_intro(parser);
	print_symbol_tree(parser->symbols, browser);
}

void	nm_print(t_nm_browser *browser, int force)
{
	int		len;
	t_header_parser *parser;

	if (force)
		browser->force = 1;
	len = ft_lstlength(browser->parsers);
	while (browser->parsers)
	{
		parser = (t_header_parser *)ft_lstpop_ptr(&browser->parsers);
		nm_print_header_parser(parser, browser, len, browser->force);
	}
}

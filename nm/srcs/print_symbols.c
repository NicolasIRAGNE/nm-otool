/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:03:11 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/17 17:51:52 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	get_debug(char debug, int has_bad_index)
{
	(void)has_bad_index;
	if (has_bad_index && (debug == 'U'))
		return ('?');
	else
		return (debug);
}

void	print_parser_header_intro(t_header_parser *parser)
{
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
		ft_printf("\n%s(%s):\n", parser->filename,
			parser->parser_union.object.name);
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
		if (!((t_nm_flags *)(browser->reserved))->flag_u)
		{
			ft_printf("%016llx %c %s\n", symbol64.nlist->n_value,
				get_debug(debug, browser->has_bad_index),
					symbol64.name);
		}
	}
	else
	{
		if (!((t_nm_flags *)(browser->reserved))->flag_u)
			ft_printf("%18s %*s\n", "U",
				(symbol64.length != -1 ? symbol64.length
					: ft_strlen(symbol64.name)),
						symbol64.name);
		else
			ft_printf("%*s\n", (symbol64.length != -1 ? symbol64.length
				: ft_strlen(symbol64.name)), symbol64.name);
	}
}

void	print_symbol32(t_symbol32 symbol32, char debug, t_browser *browser)
{
	if (((!((symbol32.nlist->n_type & N_TYPE) == N_UNDF)
			|| browser->has_bad_index)))
	{
		if (!((t_nm_flags *)(browser->reserved))->flag_u)
			ft_printf("%08llx %c %s\n", symbol32.nlist->n_value,
				get_debug(debug, browser->has_bad_index),
					symbol32.name);
	}
	else
	{
		if (!((t_nm_flags *)(browser->reserved))->flag_u)
			ft_printf("%10s %s\n", "U", symbol32.name);
		else
			ft_printf("%*s\n", (symbol32.length != -1 ? symbol32.length
				: ft_strlen(symbol32.name)), symbol32.name);
	}
}

void	print_symbol(t_header_parser *parser, t_symbol *symbol,
			t_browser *browser)
{
	if (symbol->symbol_enum == E_SYMBOL_64)
		print_symbol64(parser, symbol->symbol_union.symbol64,
			symbol->debug, browser);
	else
		print_symbol32(symbol->symbol_union.symbol32, symbol->debug, browser);
}

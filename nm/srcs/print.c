/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 13:02:13 by niragne           #+#    #+#             */
/*   Updated: 2019/06/20 19:45:42 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_parser_header_intro(t_header_parser *parser)
{
	if (parser->cputype == -1)
		ft_printf("\n%s:\n", parser->filename);
	else
		ft_printf("\n%s (for architecture %s):\n",
			parser->filename, get_cpu_name(parser->cputype));
}

/*
void	print_section64(struct section_64 *section64)
{
	ft_printf(BLUE);
	ft_printf("%s, %s\n", section64->segname, section64->sectname);
	ft_printf("Section type: %d\n", section64->flags & SECTION_TYPE);
	ft_printf("Section attributes: %b\n", section64->flags & SECTION_ATTRIBUTES);
	ft_printf(EOC);
}

void	debug_symbol_sect64(t_symbol64 symbol64, t_nm_browser *browser)
{
	if (symbol64.nlist->n_type & N_SECT)
	{
		if (symbol64.nlist->n_sect <= browser->section_arr.size)
		{
			print_section64(browser->section_arr.sections[symbol64.nlist->n_sect].section_union.section64);
		}
	}
}
*/

//void	print_symbol64_2(char debug, uint8_t n_type,
//			char *name, t_nm_browser *browser)
//{
//}

void	print_symbol64(t_symbol64 symbol64, char debug, t_nm_browser *browser)
{
	if (!((symbol64.nlist->n_type & N_TYPE) == N_UNDF) || browser->has_bad_index)
	{
		ft_printf("%016llx %c %s\n", symbol64.nlist->n_value, debug, get_symbol64_name(&symbol64));
	}
	else
	{
		ft_printf("%18s %s\n", "U", symbol64.name);
	}
}

void	print_symbol32(t_symbol32 symbol32, char debug, t_nm_browser *browser)
{
	(void)browser;

	if (!browser->has_64 || 1)
	{
		if (!((symbol32.nlist->n_type & N_TYPE) == N_UNDF)
			|| browser->has_bad_index)
		{
			ft_printf("%08llx %c %s\n", symbol32.nlist->n_value, debug, get_symbol32_name(&symbol32));
		}
		else
		{
			ft_printf("%10s %s\n", "U", get_symbol32_name(&symbol32));
		}
	}
//	else
//		print_symbol64_2(debug, symbol32.nlist->n_type,
//			symbol32.name, browser);
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
/*
void	debug_sections(t_nm_browser *browser)
{
	int i;

	i = 1;
	while (i <= browser->section_arr.size)
	{
		ft_printf("section #%d\n", i);
		print_section64(browser->section_arr.sections[i].section_union.section64);
		i++;
	}
}
*/
void	nm_print(t_header_parser *parser, t_nm_browser *browser)
{
	if (parser->symbols)
	{
		if (browser->nb_args > 1)
			print_parser_header_intro(parser);
		print_symbol_tree(parser->symbols, browser);
	}
//	debug_sections(browser);
}

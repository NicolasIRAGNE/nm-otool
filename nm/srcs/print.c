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


/*
** cf page 452 of loader.h
** return a character for a given section flag (EN VRAI je sais pas si ca suffit pour mais je pense parce quef faires des
** strcmp sur des section->sectname je pense c'est pas la bonne facon de faire apres ptet)
*/
char	get_char_from_section(uint32_t sect_flag)
{
	(void)sect_flag;
	return ('U');
}

void	print_section64(struct section_64 *section64)
{
	ft_printf("%s, %s\n", section64->segname, section64->sectname);
	ft_printf("%d\n", section64->flags & SECTION_TYPE);
}

void	debug_symbol_sect64(t_symbol64 symbol64, t_nm_browser *browser)
{
	if (symbol64.nlist->n_type & N_SECT)
	{
		if (symbol64.nlist->n_sect <= browser->section_arr.size)
		{
			print_section64(browser->section_arr.sections[symbol64.nlist->n_sect].section_union.section64);
		}
		else
		{
			//error corrupted
		}
	}
}

void	print_symbol64(t_symbol64 symbol64, t_nm_browser *browser)
{
	(void)browser;
	if (symbol64.nlist->n_value)
	{
		ft_printf("%016llx %c %s\n", symbol64.nlist->n_value,
			get_char_from_section(
				browser->section_arr.sections[symbol64.nlist->n_sect].section_union.section64->flags), symbol64.name);
	}
	else
	{
		ft_printf("%18s %s\n", "U", symbol64.name);
	}
	debug_symbol_sect64(symbol64, browser);
}

void	print_symbol32(t_symbol32 symbol32, t_nm_browser *browser)
{
	(void)browser;
	if (symbol32.nlist->n_value)
	{
		ft_printf("%08llx T %s\n", symbol32.nlist->n_value, symbol32.name);
	}
	else
	{
		ft_printf("%10s %s\n", "U", symbol32.name);
	}
}

void	print_symbol(t_symbol *symbol, t_nm_browser *browser)
{
	if (symbol->symbol_enum == E_SYMBOL_64)
		print_symbol64(symbol->symbol_union.symbol64, browser);
	else
		print_symbol32(symbol->symbol_union.symbol32, browser);
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

void	nm_print(t_nm_browser *browser)
{
	print_symbol_tree(browser->symbols, browser);
}

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

void	print_symbol64(t_symbol64 symbol64)
{
	if (symbol64.nlist->n_value)
	{
		ft_printf("%016llx T %s\n", symbol64.nlist->n_value, symbol64.name);
	}
	else
	{
		ft_printf("%18s %s\n", "U", symbol64.name);
	}
}

void	print_symbol32(t_symbol32 symbol32)
{
	if (symbol32.nlist->n_value)
	{
		ft_printf("%08llx T %s\n", symbol32.nlist->n_value, symbol32.name);
	}
	else
	{
		ft_printf("%10s %s\n", "U", symbol32.name);
	}
}

void	print_symbol(t_symbol *symbol)
{
	if (symbol->symbol_enum == E_SYMBOL_64)
		print_symbol64(symbol->symbol_union.symbol64);
	else
		print_symbol32(symbol->symbol_union.symbol32);
}

void	print_symbol_tree(t_tree *tree)
{
	if (tree)
	{
		print_symbol_tree(tree->left);
		print_symbol((t_symbol *)tree->content);
		print_symbol_tree(tree->right);
	}
}

void	nm_print(t_nm_browser *browser)
{
	print_symbol_tree(browser->symbols);
}

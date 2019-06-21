/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:35:21 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/21 01:35:21 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_symbol	*nm_new_symbol64(struct nlist_64 *nlist, char *symbol_name)
{
	t_symbol *symbol;

	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->symbol_union.symbol64.nlist = nlist;
	symbol->symbol_union.symbol64.name = symbol_name;
	symbol->symbol_enum = E_SYMBOL_64;
	return (symbol);
}

t_symbol	*nm_new_symbol32(struct nlist *nlist, char *symbol_name)
{
	t_symbol *symbol;

	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->symbol_union.symbol32.nlist = nlist;
	symbol->symbol_union.symbol32.name = symbol_name;
	symbol->symbol_enum = E_SYMBOL_32;
	return (symbol);
}

int		add_symbol_to_browser(t_nm_browser *browser, t_symbol *new_symbol)
{
	if (browser->sort_func == cmp_symbol_none)
	{
		return (ft_tree_add_sorted(&browser->symbols, new_symbol,
			browser->sort_func));
	}
	else
		return (ft_tree_add_sorted_mul(&browser->symbols, new_symbol,
			browser->sort_func, browser->sort_mult));
}

int nm_perror(char *error_message, t_nm_browser *browser)
{
	ft_dprintf(2, "%s: %s\n", browser->filename, error_message);
	return (0);
}

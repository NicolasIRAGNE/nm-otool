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

int			bad_index(char *name)
{
	if (!ft_isascii(name[0]))
		return (1);
	return (0);
}

t_symbol	*nm_new_symbol64(struct nlist_64 *nlist, char *symbol_name,
				t_nm_browser *browser)
{
	t_symbol *symbol;

	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->symbol_union.symbol64.nlist = nlist;
	symbol->symbol_union.symbol64.name = symbol_name;
	if ((symbol->symbol_union.symbol64.bad_index =
		is_corrupted_string(symbol_name, browser)))
	{
		if (browser->sort_func == cmp_symbol_alpha)
			browser->sort_func = cmp_symbol_numerical;
		browser->has_bad_index = 1;
	}
	symbol->symbol_enum = E_SYMBOL_64;
	return (symbol);
}

t_symbol	*nm_new_symbol32(struct nlist *nlist, char *symbol_name,
				t_nm_browser *browser)
{
	t_symbol *symbol;

	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->symbol_union.symbol32.nlist = nlist;
	symbol->symbol_union.symbol32.name = symbol_name;
	if ((symbol->symbol_union.symbol32.bad_index =
		is_corrupted_string(symbol_name, browser)))
	{
		if (browser->sort_func == cmp_symbol_alpha)
			browser->sort_func = cmp_symbol_numerical;
		browser->has_bad_index = 1;
	}
	symbol->symbol_enum = E_SYMBOL_32;
	return (symbol);
}

int		add_symbol_to_browser(t_header_parser *parser,
			t_nm_browser *browser, t_symbol *new_symbol)
{
	t_tree *symbol_tree;
	(void)symbol_tree;

// Parfois ignore le nouveau symbol existant, parfois non.

//	if ((symbol_tree = ft_tree_get(browser->symbols,
//			get_symbol_name(new_symbol), is_same_name_symbol)))
//	{
//		free(symbol_tree->content);
//		symbol_tree->content = new_symbol;
//		return (0);
//	}
//	else
//	{
		if (browser->sort_func == cmp_symbol_none)
		{
			return (ft_tree_add_sorted(&parser->symbols, new_symbol,
						browser->sort_func));
		}
		else
			return (ft_tree_add_sorted_mul(&parser->symbols, new_symbol,
						browser->sort_func, browser->sort_mult));
//	}
}

int nm_perror(char *error_message, t_header_parser *parser)
{
	ft_dprintf(2, "%s: %s\n", parser->filename, error_message);
	return (0);
}

int		should_add_symbol(uint8_t n_type, uint16_t n_desc, char *name,
			t_nm_browser *browser)
{
	(void)browser;
	(void)name;
	(void)n_desc;
	/*
	if (!(ft_strncmp(name, ".objc", ft_strlen(".objc"))))
	{
		ft_printf(RED);
		ft_printf("name: %s\ntype: %u\ndesc:%d\n\n", name, n_type, GET_LIBRARY_ORDINAL(n_desc));
		ft_printf(EOC);
	}
	else
		ft_printf("name: %s\ntype: %u\ndesc:%d\n\n", name, n_type, GET_LIBRARY_ORDINAL(n_desc));
	*/
	if (n_type & N_STAB )//|| GET_LIBRARY_ORDINAL(n_desc))// || n_desc) & N_DESC_DISCARDED)
		return (0);
	return (1);
}
